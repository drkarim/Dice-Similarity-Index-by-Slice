# Dice Similarity Index for 3D medical image slices
Finds the [Sorensen dice](http://en.wikipedia.org/wiki/Sørensen–Dice_coefficient) of two 3D (medical) image binary masks. 

The slices can be defined by the user using the --x, --y or, --z switches

However, it requires post-processing for finalising the atrial wall segmentation. The following repository contains two separate tools that accomplish these post-processing tasks. 

## Dependencies
Note that this program uses [MIRTK - Medical Image Registration Library](https://github.com/BioMedIA/MIRTK), an old legacy version in [IRTK](https://wwwhomes.doc.ic.ac.uk/~rkarim/mediawiki/index.php?title=Sense_Dice_Code_Ver_2) exists but is not maintained anymore. 

The dependencies are not great on MIRTK and IRTK. It is simply for accessing pixel values in the image. I envisage that the code can be easily ported into [Insight Toolkit](https://github.com/InsightSoftwareConsortium/ITK) and other imaging libraries. 

## Usage 
The usage for the first tool ```evaluate-slice-dice``` if ``make`` installed under mirtk ``/lib/tools/``:
```
./mirtk evaluate-slice-dice -i1 <img1> -i2 <img2> -o <output dice txt> -txt <append text> --x --y --z <along which direction>
```
This program outputs the mean and standard deviation of dice in each slice along the specified X, Y, Z direction. 



## Author 
```
Dr. Rashed Karim 
Department of Biomedical Engineering 
King's College London 
```
