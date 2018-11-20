#include "mirtk/Common.h"
#include "mirtk/Options.h"
#include "mirtk/IOConfig.h"
#include "mirtk/GenericImage.h"

using namespace mirtk;

Array<double> i_ctr; 
Array<double> i_pos_truth; 
Array<double> i_pos_test; 

void GetSlice(GreyImage* img1, GreyImage& img_crop, int in_x, int in_y, int in_z)
{
	int maxX, maxY, maxZ;
	maxX = img1->GetX(); 
	maxY = img1->GetY(); 
	maxZ = img1->GetZ(); 
	int n=0;

	if (in_x > 0 || in_y > 0 || in_z > 0) 
	{
		if (in_x > 0)
		{
			img_crop = img1->GetRegion(in_x, 0,0, in_x+1, maxY, maxZ); 
		}
		else if (in_y > 0)
		{
			img_crop = img1->GetRegion(0, in_y,0, maxX, in_y+1, maxZ); 
		}
		else if (in_z > 0)
		{
			img_crop = img1->GetRegion(0, 0,in_z, maxX, maxY, in_z+1);
		}
	}
}

// Return options: 
// Dice - F1 Dice measure 
// Sens - Sensitivity 
// Spec - Specificity 
// tpg - positive in ground truth 
// tpt - positive in truth
void GetSliceDice(GreyImage* img1_slice, GreyImage* img2_slice, char* which_measure, double& output)
{
	int maxX, maxY, maxZ; 
	double tot_img1=0, tot_img2=0, tot_img1_and_img2=0, false_pos=0, false_neg=0, true_neg=0; 
	
	maxX = img1_slice->GetX(); 
	maxY = img1_slice->GetY(); 
	maxZ = img1_slice->GetZ(); 

	double sorensen_dice = 0,sens, spec; 
	for (int x=0;x<maxX;x++) 
	{
		for (int y=0;y<maxY;y++)
		{
			for (int z=0;z<maxZ;z++) 
			{
				if (img1_slice->Get(x,y,z) > 0) // ground truth is positive 
				{
					tot_img1++; 
					if (img2_slice->Get(x,y,z) > 0) 	// test is also positive 
					{
						tot_img1_and_img2++; 			
					}
					else {								// but, test is negative 
						false_neg++;
					}
				}
				else if (img1_slice->Get(x,y,z) <= 0)		// ground truth is negative 
				{
					if (img2_slice->Get(x,y,z) <= 0) 
					{		
						true_neg++;						// and test is also negative
					}
					else if (img2_slice->Get(x,y,z) > 0) 	// but, test is positive 
					{
						false_pos++; 			
					}	
				}

				if (img2_slice->Get(x,y,z) > 0) 
				{
					tot_img2++;
				}
			
			}
		}
	}

	sorensen_dice = 100*(2*tot_img1_and_img2)/(tot_img1+tot_img2); 
	
	sens = (tot_img1_and_img2/(tot_img1_and_img2+false_neg))*100; 
	spec = (true_neg/(true_neg+false_pos))*100; 

	char options[][5] = {"dice", "sens", "spec", "tpg", "tpt"};	

	if (strcmp(options[0], which_measure) == 0)
	{
			output = sorensen_dice; 
	}
	else if (strcmp(options[1], which_measure) == 0)
	{
			output = sens; 
			
	}
	else if (strcmp(options[2], which_measure) == 0)
	{
			output = spec;
			
	}
	else if (strcmp(options[3], which_measure) == 0)
	{
			output = tot_img1;
			
	}
	else if (strcmp(options[4], which_measure) == 0)
	{
			output = tot_img2;
			
	}
	
	
}
double getStats(int measure, double mean)
{
	double sum=0, max=-1, t; 
	double size = i_ctr.size(); 

	if (measure == 1)			// reutnr mean 
	{
		for (int i=0;i<size;i++)
		{
			sum+=i_ctr[i]; 
		}
		return sum/size;			
	}
	if (measure == 2)			// return stdev
	{
		for (int i=0;i<size;i++)
		{
			t = i_ctr[i]; 
			sum += ((t-mean)*(t-mean));
		}
		return sqrt(sum / size);
	}
}



void ComputeSliceDiceForImages(GreyImage* img1, GreyImage* img2, ofstream& fileIO, char* appendTxt, int x_y_z, char* which_measure) 
{
	int maxX, maxY, maxZ; 
	
	double total_error, mean, std, dice_sens_spec, post_truth, pos_test; 
	maxX = img1->GetX(); 
	maxY = img1->GetY(); 
	maxZ = img1->GetZ(); 
	
	//vector<double> total_errors; 

	GreyImage img1_crop, img2_crop; 


	if (x_y_z == 1) 
	{
		// along x-direction 
		for (int x=0;x<maxX;x++)
		{
			GetSlice(img1, img1_crop, x, 0, 0);
			GetSlice(img2, img2_crop, x, 0, 0);
			
			GetSliceDice(&img1_crop, &img2_crop, which_measure, dice_sens_spec); 
			
			GetSliceDice(&img1_crop, &img2_crop, "tpg", post_truth); 
			GetSliceDice(&img1_crop, &img2_crop, "tpt", pos_test); 
			
			if (dice_sens_spec >= 0 && dice_sens_spec <= 100) {
				i_pos_truth.push_back(post_truth);
				i_pos_test.push_back(pos_test);
				i_ctr.push_back(dice_sens_spec); 
			}
			
			//fileIO << x << "\t" << dice << endl;  
			//total_errors.push_back(total_error); 
		}

	}
	
	else if (x_y_z == 2) 
	{
		// along y-direction 
		for (int y=0;y<maxY;y++)
		{
			GetSlice(img1, img1_crop, 0, y, 0);
			GetSlice(img2, img2_crop, 0, y, 0);
				
			GetSliceDice(&img1_crop, &img2_crop, which_measure, dice_sens_spec);
			GetSliceDice(&img1_crop, &img2_crop, "tpg", post_truth); 
			GetSliceDice(&img1_crop, &img2_crop, "tpt", pos_test); 
			
			if (dice_sens_spec >= 0 && dice_sens_spec <= 100) {
				i_pos_truth.push_back(post_truth);
				i_pos_test.push_back(pos_test);
				i_ctr.push_back(dice_sens_spec); 
			}
			
			//fileIO << y << "\t" << dice << endl;  
			//total_errors.push_back(total_error); 
			
		}
	}

	else if (x_y_z == 3) 
	{

		// along z-direction 
		for (int z=0;z<maxZ;z++)
		{
			GetSlice(img1, img1_crop, 0, 0, z);
			GetSlice(img2, img2_crop, 0, 0, z);

			GetSliceDice(&img1_crop, &img2_crop, which_measure, dice_sens_spec); 
			GetSliceDice(&img1_crop, &img2_crop, "tpg", post_truth); 
			GetSliceDice(&img1_crop, &img2_crop, "tpt", pos_test); 
			
			
			if (dice_sens_spec >= 0 && dice_sens_spec <= 100) {
				i_pos_truth.push_back(post_truth);
				i_pos_test.push_back(pos_test);
				i_ctr.push_back(dice_sens_spec); 
			}
			   
			//total_errors.push_back(total_error); 
			
		}
	}
	
	// compute mean and std
	mean = getStats(1, -1);
	std = getStats(2, mean); 
	//fileIO << std::setprecision(4) << appendTxt << "\t" << mean << "\t" << std << endl; 

	fileIO << "post_truth,pos_test,dice-F1\n";
	for (int i=0;i<i_ctr.size();i++)
	{
		fileIO << std::setprecision(4) << i_pos_truth[i] << "," << i_pos_test[i] << "," << i_ctr[i] << endl;
	}

}


void Binarize(RealImage* at_wall)
{
	RealPixel *p; 
	p = at_wall->GetPointerToVoxels(); 
	for (int i=0;i<at_wall->GetNumberOfVoxels();i++)
	{
		if (*p>0) 
		{ 
			*p = 1;
		}
		p++; 
	}
	//at_wall->Write("at_wall.gipl");
}


int main(int argc, char **argv)
{
	int optind;
	bool foundArgs=false;
	//GreyImage img1, img2;
	int x_y_z=1;			
	
	char* input_f1="", *input_f2="", *output_f="", *appendTxt="", *which_measure="dice";
	double t1=1, mean, stdev, total_voxels, percent;

	if (argc >= 1) 
	{
		int optind=1;
		
		while ((optind < argc) && (argv[optind][0]=='-')) {
			
			string sw = argv[optind];
			
			if (sw == "-i1") {
				optind++;
				input_f1 = argv[optind]; 
				foundArgs = true;	
			}
			
			if (sw == "-i2") {
				optind++;
				input_f2 = argv[optind]; 
				foundArgs = true;	
			}
			

			else if (sw == "-o") {
				optind++;
				output_f = argv[optind]; 
			}
				
			else if (sw == "--x") 
			{
				x_y_z = 1;
				cout << "\n\nNote: Along x"; 
			}

			else if (sw == "--y") 
			{
				x_y_z = 2;
				cout << "\n\nNote: Along y"; 
			}

			else if (sw == "--z") 
			{
				x_y_z = 3;
				cout << "\n\nNote: Along z"; 
			}
			else if (sw == "-txt")
			{
				optind++;
				appendTxt = argv[optind];
			}
			else if (sw == "--sens") 
			{
				which_measure = "sens";
				cout << "\n\nOutuput Sensitivity"; 
			}
			else if (sw == "--spec") 
			{
				which_measure = "spec";
				cout << "\n\nOutuput Specificity"; 
			}
			else if (sw == "--dice") 
			{
				which_measure = "dice";
				cout << "\n\nOutput Dice"; 
			}
			
			
			optind++; 
		}
	}
	
	if (foundArgs == false)
	{
		cout << "\n\nUsage: slicedice_mean.exe \n\n\t-i1 <truth IMG> \n\t-i2 <test IMG> \n\t-o <output dice txt> \n\t-txt <append text to file> \n\nOther Options\n=============\n\t--x --y --z <along which direction>\n\t--dice --sens --spec <Dice, sensitivity and specificity measure>" << endl; 
		exit(0); 
	}

	else
	{
		InitializeIOLibrary();
		//GreyImage img1(input_f1);
		//GreyImage img2(input_f2); 

		UniquePtr<BaseImage> img1(BaseImage::New(input_f1));
		UniquePtr<BaseImage> img2(BaseImage::New(input_f2));


		/*
		img1.Read(input_f1); 
		img2.Read(input_f2);*/

		// check images are same size 
		if (!(img1->GetX() == img2->GetX() && img1->GetY() == img2->GetY() && img1->GetZ() == img2->GetZ()))
		{
			cout << "Image size mismatch, images need to be of equal dimensions! " << endl; 
			exit(0); 
		}
		
		ofstream fileIO; 
		fileIO.open(output_f, std::ios_base::app); 
		
		ComputeSliceDiceForImages(new GenericImage<short>(*img1), new GenericImage<short>(*img2), fileIO, appendTxt,  x_y_z, which_measure);
		fileIO.close();
	}

}
