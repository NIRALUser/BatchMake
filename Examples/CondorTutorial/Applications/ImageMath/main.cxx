/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    main.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include <cmath>
#include <iostream>

#include "itkImage.h"
#include "itkImageFileWriter.h"
#include "itkImageFileReader.h"
#include "itkMetaImageIO.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkNJetImageFunction.h"
#include "itkNormalVariateGenerator.h"
#include "itkHistogramMatchingImageFilter.h"
#include "itkResampleImageFilter.h"
#include "itkCastImageFilter.h"
#include "itkRecursiveGaussianImageFilter.h"
#include "itkNearestNeighborInterpolateImageFunction.h"
#include "itkErodeObjectMorphologyImageFilter.h"
#include "itkBinaryBallStructuringElement.h"
#include "itkDilateObjectMorphologyImageFilter.h"
#include "itkConnectedThresholdImageFilter.h"
#include "itkCVTImageFilter.h"
#include "metaCommand.h"

/*
int usage(void)
  {
  std::cout << "ImageMath <infile> [options]  " << std::endl;
  std::cout << "  - note: options are applied in cmdline order" << std::endl;
  std::cout << "Options :" << std::endl;
  std::cout << "  -w <imageOutputFile>"
            << std::endl
            << "     : writes the current image to the designated file." 
            << std::endl
            << "       Image is stored as floats" 
            << std::endl;
  std::cout << "  -W <type> <imageOutputFile>"
            << std::endl
            << "     : writes the current image to the designated file." 
            << std::endl
            << "       Using pixel type (0=unsigned byte, 1=unsigned" 
            << std::endl
            << "       short, 2=short, 3=formatted for MIDAS)" 
            << std::endl;
  std::cout << "  -i <inValMin> <inValMax> <outMin> <outMax>"
            << std::endl
            << "     : Intensity window inVal range to outValRange "
            << std::endl
            << "       - with clipping" 
            << std::endl;
  std::cout << "  -I <inMeanField>"
            << std::endl
            << "     : Intensity multiplicative correct using inMeanField"
            << std::endl;
  std::cout << "  -n <inValMin> <inValMax> <noiseMean> <noiseStdDev>"
            << std::endl
            << "     : Adds Gaussian noise to all pixels within inVal range." 
            << std::endl;
  std::cout << "  -N <inValMin> <inValMax> <noiseMean> <noiseRange>"
            << std::endl
            << "     : Adds uniform noise to all pixels within inVal range." 
            << std::endl
            << "     : Noise is uniform within noiseMean+/-noiseRange"
            << std::endl;
  std::cout << "  -a <weight1> <weight2> <inFile2>"
            << std::endl
            << "     : I(x) = weight1*I(x) + weight2*inFile2(x)" 
            << std::endl;
  std::cout << "  -f <offset2> <inFile2>"
            << std::endl
            << "     : fuse two images." << std::endl;
            << "     : C(x) = argmax(C=1..2)(inFileC(x))." << std::endl;
            << "     : I(x) = (C==1)?(inFile1(x)):(offset2+inFile2(x))." 
            << std::endl;
            << std::endl;
  std::cout << "  -t <threshLow> <threshHigh> <valTrue> <valFalse>"
            << std::endl
            << "         if threshLow <= I(x) <= threshHigh "
            << std::endl
            << "             then I(x)=valTrue else I(x)=valFalse" 
            << std::endl;
  std::cout << "  -m <threshLow> <threshHigh> <inFile2> <valFalse>"
            << std::endl
            << "         if threshLow <= inFile21(x) <= threshHigh "
            << std::endl
            << "             then I(x)=I(x) else I(x)=valFalse" 
            << std::endl;
  std::cout << "  -M <mode> <radius> <forgroundValue> <backgroundValue>"
            << std::endl
            << "     : Mathematical morphology using a sphere.  Foreground"
            << std::endl
            << "       voxels are morphed (mode 0=erode, 1=dilate)."
            << std::endl
            << "       Erosion fills-in using backgroundValue." 
            << std::endl;
  std::cout << "  -b <sigma>"
            << std::endl
            << "     : gaussian blur the image using the given sigma" 
            << std::endl;
  std::cout << "  -B <sigma> <order> <direction>"
            << std::endl
            << "     : gaussian blur the image using the given sigma" 
            << std::endl
            << "       and the order (0 = blur, 1 = 1st derivative,"
            << std::endl
            << "       and 2 = 2nd derivative) and in the direction"
            << std::endl
            << "       given (0 = x, 1 = y, ...)."
            << std::endl;
  std::cout << "  -h <nBins> <histOutputFile>"
            << std::endl
            << "     : writes the image's histogram to the designated file" 
            << std::endl;
  std::cout << "  -H <nBins> <binMin> <binSIZE> <histOutputFile>"
            << std::endl
            << "     : writes the image's histogram to the designated file" 
            << std::endl;
  std::cout << "  -v <scaleMin> <scaleMax> <step> (3D only)"
            << std::endl
            << "     : Enhance vessels using specified scales"
            << std::endl;
  std::cout << "  -c <nBins> <nMatchPoints> (3D only)"
            << std::endl
            << "     : Correct intensity slice-by-slice" 
            << std::endl
            << "     : NBins & nMatchPoints are used by HistogramMatchingFilter"
            << std::endl;
  std::cout << "  -C <nBins> <nMatchPoints> <referenceVolume>"
            << std::endl
            << "     : Correct intensity to match another volume's" 
            << std::endl
            << "     : NBins & nMatchPoints are used by HistogramMatchingFilter"
            << std::endl;
  std::cout << "  -r factor"
            << std::endl
            << "     : resample an image to reduce dim size by a factor"
            << std::endl
            << "       (if factor==0, then make isotropic)"
            << std::endl;
  std::cout << "  -R <inFile2>"
            << std::endl
            << "     : resample an image to match the dimensions of inFile2"
            << std::endl;
  std::cout << "  -s <mode> <threshLow> <threshHigh> <labelValue> <x> <y> <z>"
            << std::endl
            << "     : mode=0 - segment using threshold connected components"
            << std::endl
            << "       from seed (x,y,z) label connected points having "
            << std::endl
            << "       intensities bounded by (inclusive) the thresholds"
            << std::endl;
  std::cout << "  -O <offsetX> <offsetY> <offsetZ>"
            << "     : Set the image offset to the given values"
            << std::endl;
  std::cout << "  -S <seedValue>"
            << std::endl
            << "     : Sets the random number seed - to repeat experiments" 
            << std::endl;
  std::cout << "  -V <numberOfCentroids> <numberOfIterations> <numberOfSamples> <centroidOutputFile>"
            << std::endl
            << "     : Run centroid voronoi tessellation on the image" 
            << std::endl;
  return 0;
  }*/

/** Typdefs */
typedef float PixelType;
typedef itk::Image<PixelType, 3> ImageType;

typedef itk::Image<unsigned char, 3> ImageTypeUChar;
typedef itk::Image<unsigned short, 3> ImageTypeUShort;
typedef itk::Image<short, 3> ImageTypeShort;

/** Resamples image a to b if they are different, returns resampled_a */
ImageType::Pointer ResampleImage( ImageType::Pointer a, ImageType::Pointer b )
{
  ImageType::Pointer output = a;
  
  for( int i = 0; i < 3; i++ )
    {
    if( a->GetLargestPossibleRegion().GetSize()[i] 
          != b->GetLargestPossibleRegion().GetSize()[i] 
        || a->GetSpacing()[i] != b->GetSpacing()[i] 
        || a->GetOrigin()[i] != b->GetOrigin()[i]  )
      {
      typedef itk::ResampleImageFilter<ImageType,ImageType> ResampleFilterType;
      ResampleFilterType::Pointer filter = ResampleFilterType::New();
      filter->SetInput( a );
      filter->SetSize( b->GetLargestPossibleRegion().GetSize() );
      filter->SetOutputOrigin( b->GetOrigin() );
      filter->SetOutputSpacing( b->GetSpacing() );
      filter->SetDefaultPixelValue(0);
      filter->Update();
      output = filter->GetOutput();
      }
    }
  return output;
}

/** Main command */
int main(int argc, char **argv)
  {
  MetaCommand command;
  
  command.SetName("ImageMath");
  command.SetVersion("1.0");
  command.SetAuthor("CADDLab @ UNC");
  command.SetDescription("Perform several filters on an image");

  command.AddField("infile","infile filename",MetaCommand::STRING,MetaCommand::DATA_IN);

  command.SetOption("Intensity","i",false,"Intensity window inVal range to outValRange");
  command.AddOptionField("Intensity","inValMin",MetaCommand::FLOAT,true);
  command.AddOptionField("Intensity","inValMax",MetaCommand::FLOAT,true);
  command.AddOptionField("Intensity","outMin",MetaCommand::FLOAT,true);
  command.AddOptionField("Intensity","outMax",MetaCommand::FLOAT,true);

  command.SetOption("IntensityMult","I",false,"Intensity multiplicative correct using inMeanField");
  command.AddOptionField("IntensityMult","inMeanField",MetaCommand::INT,true);

  command.SetOption("GaussianNoise","n",false,"Adds Gaussian noise to all pixels within inVal range");
  command.AddOptionField("GaussianNoise","inValMin",MetaCommand::FLOAT,true);
  command.AddOptionField("GaussianNoise","inValMax",MetaCommand::FLOAT,true);
  command.AddOptionField("GaussianNoise","noiseMean",MetaCommand::FLOAT,true);
  command.AddOptionField("GaussianNoise","noiseStdDev",MetaCommand::FLOAT,true);

  command.SetOption("UniformNoise","N",false,"Adds uniform noise to all pixels within inVal range");
  command.AddOptionField("UniformNoise","inValMin",MetaCommand::FLOAT,true);
  command.AddOptionField("UniformNoise","inValMax",MetaCommand::FLOAT,true);
  command.AddOptionField("UniformNoise","noiseMean",MetaCommand::FLOAT,true);
  command.AddOptionField("UniformNoise","noiseRange",MetaCommand::FLOAT,true);

  command.SetOption("Fuse","f",false,"fuse two images by max, applying offset to second image");
  command.AddOptionField("Fuse","Offset2",MetaCommand::FLOAT,true);
  command.AddOptionField("Fuse","Infile2",MetaCommand::STRING,true);

  command.SetOption("Add","a",false,"I(x) = weight1*I(x) + weight2*inFile2(x)");
  command.AddOptionField("Add","weight1",MetaCommand::FLOAT,true);
  command.AddOptionField("Add","weight2",MetaCommand::FLOAT,true);
  command.AddOptionField("Add","Infile",MetaCommand::STRING,true);

  command.SetOption("Threshold","t",false,"if threshLow <= I(x) <= threshHigh then I(x)=valTrue else I(x)=valFalse");
  command.AddOptionField("Threshold","threshLow",MetaCommand::FLOAT,true);
  command.AddOptionField("Threshold","threshHigh",MetaCommand::FLOAT,true);
  command.AddOptionField("Threshold","valTrue",MetaCommand::FLOAT,true);
  command.AddOptionField("Threshold","valFalse",MetaCommand::FLOAT,true);

  command.SetOption("Masking","m",false,"if threshLow <= inFile21(x) <= threshHigh then I(x)=I(x) else I(x)=valFalse");
  command.AddOptionField("Masking","threshLow",MetaCommand::FLOAT,true);
  command.AddOptionField("Masking","threshHigh",MetaCommand::FLOAT,true);
  command.AddOptionField("Masking","inFile2",MetaCommand::STRING,true);
  command.AddOptionField("Masking","valFalse",MetaCommand::FLOAT,true);

  command.SetOption("Morphology","M",false,"Mathematical morphology using a sphere.  Foreground voxels are morphed (mode 0=erode, 1=dilate)).Erosion fills-in using backgroundValue.");
  command.AddOptionField("Morphology","mode",MetaCommand::INT,true);
  command.AddOptionField("Morphology","radius",MetaCommand::FLOAT,true);
  command.AddOptionField("Morphology","forgroundValue",MetaCommand::FLOAT,true);
  command.AddOptionField("Morphology","backgroundValue",MetaCommand::FLOAT,true);

  command.SetOption("blur","b",false,"gaussian blur the image using the given sigma");
  command.AddOptionField("blur","sigma",MetaCommand::FLOAT,true);

  command.SetOption("blurOrder","B",false,"gaussian blur the image using the given sigma and the order.");
  command.AddOptionField("blurOrder","sigma",MetaCommand::FLOAT,true);
  command.AddOptionField("blurOrder","order",MetaCommand::INT,true);
  command.AddOptionField("blurOrder","direction",MetaCommand::INT,true);

  command.SetOption("histogram","h",false,"writes the image's histogram to the designated file");
  command.AddOptionField("histogram","nBins",MetaCommand::INT,true);
  command.AddOptionField("histogram","histOutputFile",MetaCommand::STRING,true);

  command.SetOption("histogram2","H",false,"writes the image's histogram to the designated file");
  command.AddOptionField("histogram2","nBins",MetaCommand::INT,true);
  command.AddOptionField("histogram2","binMin",MetaCommand::FLOAT,true);
  command.AddOptionField("histogram2","binSIZE",MetaCommand::FLOAT,true);
  command.AddOptionField("histogram2","histOutputFile",MetaCommand::STRING,true);

  command.SetOption("vessels","v",false,"Enhance vessels using specified scales");
  command.AddOptionField("vessels","scaleMin",MetaCommand::INT,true);
  command.AddOptionField("vessels","scaleMax",MetaCommand::INT,true);
  command.AddOptionField("vessels","step",MetaCommand::INT,true);

  command.SetOption("CorrectionSlice","c",false,"Correct intensity slice-by-slice NBins & nMatchPoints are used by HistogramMatchingFilter");
  command.AddOptionField("CorrectionSlice","nBins",MetaCommand::INT,true);
  command.AddOptionField("CorrectionSlice","nMatchPoints",MetaCommand::INT,true);

  command.SetOption("Correction","C",false,"Correct intensity to match another volume's NBins & nMatchPoints are used by HistogramMatchingFilter");
  command.AddOptionField("Correction","nBins",MetaCommand::INT,true);
  command.AddOptionField("Correction","nMatchPoints",MetaCommand::INT,true);
  command.AddOptionField("Correction","referenceVolume",MetaCommand::STRING,true);

  command.SetOption("resize","r",false,"resample an image to reduce dim size by a factor (if factor==0, then make isotropic)");
  command.AddOptionField("resize","factor",MetaCommand::FLOAT,true);
  
  command.SetOption("resize2","R",false,"resample an image to match the dimensions of inFile2");
  command.AddOptionField("resize2","inFile2",MetaCommand::STRING,true);

  command.SetOption("segment","s",false,"mode=0 - segment using threshold connected components from seed (x,y,z) label connected points having intensities bounded by (inclusive) the thresholds");
  command.AddOptionField("segment","mode",MetaCommand::INT,true);
  command.AddOptionField("segment","threshLow",MetaCommand::FLOAT,true);
  command.AddOptionField("segment","threshHigh",MetaCommand::FLOAT,true);
  command.AddOptionField("segment","labelValue",MetaCommand::FLOAT,true);
  command.AddOptionField("segment","x",MetaCommand::FLOAT,true);
  command.AddOptionField("segment","y",MetaCommand::FLOAT,true);
  command.AddOptionField("segment","z",MetaCommand::FLOAT,true);

  command.SetOption("offset","O",false,"resample an image to match the dimensions of inFile2");
  command.AddOptionField("offset","offsetX",MetaCommand::FLOAT,true);
  command.AddOptionField("offset","offsetY",MetaCommand::FLOAT,true);
  command.AddOptionField("offset","offsetZ",MetaCommand::FLOAT,true);
  
  command.SetOption("SetRandom","S",false,"Sets the random number seed - to repeat experiments");
  command.AddOptionField("SetRandom","seedValue",MetaCommand::FLOAT,true);

  command.SetOption("Voronoi","V",false,"Run centroid voronoi tessellation on the image");
  command.AddOptionField("Voronoi","numberOfCentroids",MetaCommand::FLOAT,true);
  command.AddOptionField("Voronoi","numberOfIterations",MetaCommand::FLOAT,true);
  command.AddOptionField("Voronoi","numberOfSamples",MetaCommand::FLOAT,true);
  command.AddOptionField("Voronoi","centroidOutputFile",MetaCommand::STRING,true);

  command.SetOption("Write","w",false,"writes the current image to the designated file");
  command.AddOptionField("Write","filename",MetaCommand::STRING,true,"","output filename",MetaCommand::DATA_OUT);

  command.SetOption("WriteType","W",false,"writes the current image to the designated file with a type : 0=unsigned char, 1=unsignedShort, 2=short, 3=formated for MIDAS");
  command.AddOptionField("WriteType","Type",MetaCommand::INT,true);
  command.AddOptionField("WriteType","filename",MetaCommand::STRING,true,"","output filename",MetaCommand::DATA_OUT);

  // Parsing
  if(!command.Parse(argc,argv))
    {
    return 1;
    }

  MetaCommand::OptionVector parsed = command.GetParsedOptions();

  typedef itk::Statistics::NormalVariateGenerator gaussGenType;
  gaussGenType::Pointer gaussGen = gaussGenType::New();

  typedef itk::ImageFileReader< ImageType > VolumeReaderType;
  typedef itk::NJetImageFunction< ImageType > ImageFunctionType;

  // Declare a reader
  VolumeReaderType::Pointer reader = VolumeReaderType::New();
  reader->SetFileName( command.GetValueAsString("infile").c_str() );
  ImageType::Pointer imIn;
  imIn = reader->GetOutput();
  
  // See if the file can be read - "try" otherwise program will 
  //   mysteriously exit on failure in the Object factory
  std::cout << "Reading file (" << command.GetValueAsString("infile").c_str() <<")"<< std::endl;
  try
    {
    reader->Update();
    }
  catch( ... )
    {
    std::cout << "Problems reading file format" << std::endl;
    return 1;
    }

  MetaCommand::OptionVector::const_iterator it = parsed.begin();
  while(it != parsed.end())
    {
    if((*it).name == "Write")
      {
      std::string outFilename = command.GetValueAsString(*it,"filename");
      std::cout << "Writing output1 (" << outFilename.c_str() << ")" << std::endl;
      typedef itk::ImageFileWriter< ImageType > VolumeWriterType;
      VolumeWriterType::Pointer writer = VolumeWriterType::New();
      writer->SetFileName( outFilename.c_str() );
      writer->SetInput( imIn );
      writer->SetUseCompression(true);
      try
        {
        writer->Write();
        }
      catch( itk::ExceptionObject& err )
        {
        std::cout << "WriteImage : " << err << std::endl;
        return 1;  
        }
      } // end -w

    else if((*it).name == "WriteType")
      {
      int type = command.GetValueAsInt("WriteType","Type");
      std::string outFilename = command.GetValueAsString(*it,"filename");
      std::cout << "Writing output2 (" << outFilename.c_str() << ")" << std::endl;
      switch(type)
        {
          case 0:
            {
            typedef itk::CastImageFilter< ImageType, ImageTypeUChar> 
                  CastFilterType;
            CastFilterType::Pointer castFilter = CastFilterType::New();
            castFilter->SetInput( imIn );

            typedef itk::ImageFileWriter< ImageTypeUChar > VolumeWriterType;
            VolumeWriterType::Pointer writer = VolumeWriterType::New();
            writer->SetFileName( outFilename.c_str() );
            writer->SetInput( castFilter->GetOutput() );
            writer->SetUseCompression(true);
            writer->Write();
            break;
            }
          case 1:
            {
            typedef itk::CastImageFilter< ImageType, ImageTypeUShort> 
                  CastFilterType;
            CastFilterType::Pointer castFilter = CastFilterType::New();
            castFilter->SetInput( imIn );

            typedef itk::ImageFileWriter< ImageTypeUShort > VolumeWriterType;
            VolumeWriterType::Pointer writer = VolumeWriterType::New();
            writer->SetFileName( outFilename.c_str() );
            writer->SetInput( castFilter->GetOutput() );
            writer->SetUseCompression(true);
            writer->Write();
            break;
            }
          case 2:
            {
            typedef itk::CastImageFilter< ImageType, ImageTypeShort> 
                  CastFilterType;
            CastFilterType::Pointer castFilter = CastFilterType::New();
            castFilter->SetInput( imIn );

            typedef itk::ImageFileWriter< ImageTypeShort > VolumeWriterType;
            VolumeWriterType::Pointer writer = VolumeWriterType::New();
            writer->SetFileName( outFilename.c_str() );
            writer->SetInput( castFilter->GetOutput() );
            writer->SetUseCompression(true);
            writer->Write();
            break;
            }
          case 3:
            {
            typedef itk::CastImageFilter< ImageType, ImageTypeShort> 
                  CastFilterType;
            CastFilterType::Pointer castFilter = CastFilterType::New();
            castFilter->SetInput( imIn );

            typedef itk::ImageFileWriter< ImageTypeShort > VolumeWriterType;
            VolumeWriterType::Pointer writer = VolumeWriterType::New();

            itk::MetaImageIO::Pointer metaWriter = itk::MetaImageIO::New();
            writer->SetImageIO( metaWriter );

            writer->SetFileName( outFilename.c_str() );
            writer->SetInput( castFilter->GetOutput() );
            writer->SetUseCompression(false);

            MetaImage * metaImage = metaWriter->GetMetaImagePointer();

            metaImage->ElementSize(0, imIn->GetSpacing()[0]);
            metaImage->ElementSize(1, imIn->GetSpacing()[1]);
            metaImage->ElementSize(2, imIn->GetSpacing()[2]);

            metaImage->AddUserField("ElementByteOrderMSB",
                                    MET_STRING, strlen("False"), "False");

            writer->Write();
            break;
            }
          }
        } // end -W

        else if((*it).name == "Intensity")
        {
        std::cout << "Intensity windowing" << std::endl;
        float valMin = command.GetValueAsFloat(*it,"inValMin");
        float valMax = command.GetValueAsFloat(*it,"inValMax");
        float outMin = command.GetValueAsFloat(*it,"outMin");
        float outMax = command.GetValueAsFloat(*it,"outMax");
        itk::ImageRegionIterator< ImageType > it(imIn, 
              imIn->GetLargestPossibleRegion());
        it.GoToBegin();
        while(!it.IsAtEnd())
          {
          double tf = it.Get();
          tf = (tf-valMin)/(valMax-valMin);
          if(tf<0)
            {
            tf = 0;
            }
          if(tf>1)
            {
            tf = 1;
            }
          tf = (tf * (outMax-outMin)) + outMin;
          it.Set((PixelType)tf);
          ++it;
          }
        }

      // IntensityMult
      else if((*it).name == "IntensityMult")
        {
        std::cout << "Intensity multiplicative bias correct" << std::endl;
        VolumeReaderType::Pointer reader2 = VolumeReaderType::New();
        reader2->SetFileName( command.GetValueAsString(*it,"inMeanField").c_str() );
        ImageType::Pointer imIn2;
        imIn2 = reader2->GetOutput();
        try
          {
          reader2->Update();
          }
        catch( ... )
          {
          std::cout << "Problems reading file format of inFile2." << std::endl;
          return 1;
          }
        imIn2 = ResampleImage(imIn2, imIn);
        itk::ImageRegionIterator< ImageType > it2(imIn2, 
                       imIn2->GetLargestPossibleRegion());
        int count = 0;
        double mean = 0;
        it2.GoToBegin();
        while(!it2.IsAtEnd())
          {
          double tf = it2.Get();
          mean += tf;
          if(tf != 0)
            {
            ++count;
            }
          ++it2;
          }
        mean /= count;
        itk::ImageRegionIterator< ImageType > it(imIn, 
              imIn->GetLargestPossibleRegion());
        it.GoToBegin();
        it2.GoToBegin();
        while(!it.IsAtEnd())
          {
          double tf = it.Get();
          double tf2 = it2.Get();
          if(tf2 != 0)
            {
            double alpha = mean / tf2;
            tf = tf * alpha;
            it.Set((PixelType)tf);
            }
          ++it;
          ++it2;
          }
        } // end -I

      // UniformNoise
      else if((*it).name == "UniformNoise")
        {
        std::cout << "Adding noise" << std::endl;
        float valMin = command.GetValueAsFloat(*it,"inValMin");
        float valMax = command.GetValueAsFloat(*it,"inValMax");
        float noiseMean = command.GetValueAsFloat(*it,"noiseMean");
        float noiseRange = command.GetValueAsFloat(*it,"noiseRange");        
        itk::ImageRegionIterator< ImageType > it(imIn, 
              imIn->GetLargestPossibleRegion());
        it.GoToBegin();
        while(!it.IsAtEnd())
          {
          double tf = it.Get();
          if(tf >= valMin && tf <= valMax)
            {
            tf += (2.0*(rand()/(double)RAND_MAX)-1)*noiseRange+noiseMean;
            it.Set((PixelType)tf);
            }
          ++it;
          }
        } // -N
     
      // GaussianNoise
      else if((*it).name == "GaussianNoise")
        {
        std::cout << "Adding noise" << std::endl;
        float valMin = command.GetValueAsFloat(*it,"inValMin");
        float valMax = command.GetValueAsFloat(*it,"inValMax");
        float noiseMean = command.GetValueAsFloat(*it,"noiseMean");
        float noiseStdDev = command.GetValueAsFloat(*it,"noiseStdDev");
        itk::ImageRegionIterator< ImageType > it(imIn, 
              imIn->GetLargestPossibleRegion());
        it.GoToBegin();
        while(!it.IsAtEnd())
          {
          double tf = it.Get();
          if(tf >= valMin && tf <= valMax)
            {
            tf += gaussGen->GetVariate()*noiseStdDev+noiseMean;
            it.Set((PixelType)tf);
            }
          ++it;
          }
        } // end -n
      
      // I(x)
      else if((*it).name == "Add")
        {
        std::cout << "Adding" << std::endl;
        float weight1 = command.GetValueAsFloat(*it,"weight1");
        float weight2 = command.GetValueAsFloat(*it,"weight2");
        VolumeReaderType::Pointer reader2 = VolumeReaderType::New();
        reader2->SetFileName( command.GetValueAsString(*it,"Infile").c_str() );
        ImageType::Pointer imIn2;
        imIn2 = reader2->GetOutput();
        try
          {
          reader2->Update();
          }
        catch( ... )
          {
          std::cout << "Problems reading file format of inFile2." << std::endl;
          return 1;
          }
        itk::ImageRegionIterator< ImageType > it1(imIn, 
              imIn->GetLargestPossibleRegion());
        itk::ImageRegionIterator< ImageType > it2(imIn2, 
              imIn2->GetLargestPossibleRegion());
        it1.GoToBegin();
        it2.GoToBegin();
        while(!it1.IsAtEnd())
          {
          double tf1 = it1.Get();
          double tf2 = it2.Get();
          double tf = weight1*tf1 + weight2*tf2;
          it1.Set((PixelType)tf);
          ++it1;
          ++it2;
          }
        }
      // I(x)
      else if((*it).name == "Fuse")
        {
        std::cout << "Fusing" << std::endl;
        float offset2 = command.GetValueAsFloat(*it,"Offset2");
        VolumeReaderType::Pointer reader2 = VolumeReaderType::New();
        reader2->SetFileName( command.GetValueAsString(*it,"Infile2").c_str() );
        ImageType::Pointer imIn2;
        imIn2 = reader2->GetOutput();
        try
          {
          reader2->Update();
          }
        catch( ... )
          {
          std::cout << "Problems reading file format of inFile2." << std::endl;
          return 1;
          }
        itk::ImageRegionIterator< ImageType > it1(imIn, 
              imIn->GetLargestPossibleRegion());
        itk::ImageRegionIterator< ImageType > it2(imIn2, 
              imIn2->GetLargestPossibleRegion());
        it1.GoToBegin();
        it2.GoToBegin();
        while(!it1.IsAtEnd())
          {
          double tf1 = it1.Get();
          double tf2 = it2.Get();
          if(tf2>tf1)
            {
            double tf = offset2 + tf2;
            it1.Set((PixelType)tf);
            }
          ++it1;
          ++it2;
          }
        } // end -a
      
      // Threshold
      else if((*it).name == "Threshold")
        {
        std::cout << "Thresholding" << std::endl;

        float threshLow = command.GetValueAsFloat(*it,"threshLow");
        float threshHigh = command.GetValueAsFloat(*it,"threshHigh");

        float valTrue = command.GetValueAsFloat(*it,"valTrue");
        float valFalse = command.GetValueAsFloat(*it,"valFalse");

        itk::ImageRegionIterator< ImageType > it1(imIn, 
              imIn->GetLargestPossibleRegion());
        it1.GoToBegin();
        while(!it1.IsAtEnd())
          {
          double tf1 = it1.Get();
          if(tf1 >= threshLow && tf1 <= threshHigh)
            {
            it1.Set((PixelType)valTrue);
            }
          else
            {
            it1.Set((PixelType)valFalse);
            }
          ++it1;
          }
        }

      // Masking
      else if((*it).name == "Masking")
        {
        std::cout << "Masking" << std::endl;

        float threshLow = command.GetValueAsFloat(*it,"threshLow");
        float threshHigh = command.GetValueAsFloat(*it,"threshHigh");

        VolumeReaderType::Pointer reader2 = VolumeReaderType::New();
        reader2->SetFileName( command.GetValueAsString(*it,"inFile2").c_str() );

        float valFalse = command.GetValueAsFloat(*it,"valFalse");

        ImageType::Pointer imIn2 = reader2->GetOutput();
        try
          {
          reader2->Update();
          }
        catch( ... )
          {
          std::cout << "Problems reading file format of inFile2." << std::endl;
          return 1;
          }
        imIn2 = ResampleImage( imIn2, imIn );
        itk::ImageRegionIterator< ImageType > it1(imIn, 
              imIn->GetLargestPossibleRegion());
        itk::ImageRegionIterator< ImageType > it2(imIn2, 
              imIn2->GetLargestPossibleRegion());
        it1.GoToBegin();
        it2.GoToBegin();
        while(!it1.IsAtEnd())
          {
          double tf2 = it2.Get();
          if(tf2 >= threshLow && tf2 <= threshHigh)
            {
            //double tf1 = it1.Get();
            //it1.Set((PixelType)tf1);
            }
          else
            {
            it1.Set((PixelType)valFalse);
            }
          ++it1;
          ++it2;
          }
        }

      // Morphology
      else if((*it).name == "Morphology")
        {
        std::cout << "Morphology" << std::endl;

        int mode = command.GetValueAsInt(*it,"mode");

        float radius = command.GetValueAsFloat(*it,"radius");

        float foregroundValue = command.GetValueAsFloat(*it,"forgroundValue");
        float backgroundValue = command.GetValueAsFloat(*it,"backgroundValue");

        typedef itk::BinaryBallStructuringElement<PixelType, 3>  BallType;
        BallType ball;
        ball.SetRadius( 1 );
        ball.CreateStructuringElement();

        typedef itk::ErodeObjectMorphologyImageFilter
                     <ImageType, ImageType, BallType>       ErodeFilterType;
        typedef itk::DilateObjectMorphologyImageFilter
                     <ImageType, ImageType, BallType>       DilateFilterType;
        switch(mode)
          {
          case 0:
            {
            for(int r=0; r<radius; r++)
              {
              ErodeFilterType::Pointer filter = ErodeFilterType::New();
              filter->SetBackgroundValue(backgroundValue);
              filter->SetKernel( ball );
              filter->SetObjectValue(foregroundValue);
              filter->SetInput( imIn );
              filter->Update();
              imIn = filter->GetOutput();
              }
            break;
            }
          case 1:
            {
            for(int r=0; r<radius; r++)
              {
              DilateFilterType::Pointer filter = DilateFilterType::New();
              filter->SetKernel( ball );
              filter->SetObjectValue(foregroundValue);
              filter->SetInput( imIn );
              filter->Update();
              imIn = filter->GetOutput();
              }
            break;
            }
          }
        }

      // blur
      else if((*it).name == "blur")
        {
        std::cout << "Blurring." << std::endl;
        float sigma = command.GetValueAsFloat(*it,"sigma");

        itk::RecursiveGaussianImageFilter< ImageType >::Pointer filter;
        ImageType::Pointer imTemp;
        for(int i=0; i<3; i++)
          {
          filter = itk::RecursiveGaussianImageFilter< ImageType >::New();
          filter->SetInput( imIn );
          // filter->SetNormalizeAcrossScale( true );
          filter->SetSigma( sigma );
  
          filter->SetOrder( 
                   itk::RecursiveGaussianImageFilter<ImageType>::ZeroOrder );
          filter->SetDirection( i );

          imTemp = filter->GetOutput();
          filter->Update();
          imIn = imTemp;
          }
        }

      // blurOrder
      else if((*it).name == "blurOrder")
        {
        std::cout << "Blurring." << std::endl;
 
        float sigma = command.GetValueAsFloat(*it,"sigma");
        int order = command.GetValueAsInt(*it,"order");
        int direction = command.GetValueAsInt(*it,"direction");

        itk::RecursiveGaussianImageFilter< ImageType >::Pointer filter;
        filter = itk::RecursiveGaussianImageFilter< ImageType >::New();
        filter->SetInput( imIn );
        // filter->SetNormalizeAcrossScale( true );
        filter->SetSigma( sigma );
        filter->SetDirection( direction );
        switch(order)
          {
          case 0:
            filter->SetOrder( 
                    itk::RecursiveGaussianImageFilter<ImageType>::ZeroOrder );
            break;
          case 1:
            filter->SetOrder( 
                    itk::RecursiveGaussianImageFilter<ImageType>::FirstOrder );
            break;
          case 2:
            filter->SetOrder( 
                    itk::RecursiveGaussianImageFilter<ImageType>::SecondOrder );
            break;
          }
        ImageType::Pointer imTemp = filter->GetOutput();
        filter->Update();
        imIn = imTemp;
        } // end -B

      // histogram
      else if((*it).name == "histogram")
        {
        std::cout << "Histogram" << std::endl;

        unsigned int nBins = (unsigned int)command.GetValueAsInt(*it,"nBins");
        const char * filename = command.GetValueAsString(*it,"histOutputFile").c_str();

        itk::ImageRegionIteratorWithIndex< ImageType > it1(imIn, 
              imIn->GetLargestPossibleRegion());
        it1.GoToBegin();
        double binMin = it1.Get();
        double binMax = it1.Get();
        while(!it1.IsAtEnd())
          {
          double tf = it1.Get();
          if(tf < binMin)
            {
            binMin = tf;
            }
          else
            if(tf > binMax)
              {
              binMax = tf;
              }
          ++it1;
          }
        std::cout << "  binMin = " << binMin << std::endl;
        std::cout << "  binMax = " << binMax << std::endl;
        it1.GoToBegin();
        itk::Array<double> bin;
        bin.set_size(nBins);
        bin.Fill(0);
        while(!it1.IsAtEnd())
          {
          double tf = it1.Get();
          tf = (tf-binMin)/(binMax-binMin) * nBins;
          if(tf>nBins-1)
            {
            tf = nBins-1;
            }
          else
            if(tf<0)
              {
              tf = 0;
              }
          bin[(int)tf]++;
          ++it1;
          }
        std::ofstream writeStream;
        writeStream.open(filename, std::ios::binary | std::ios::out);
        for(unsigned int i=0; i<nBins; i++)
          {
          writeStream << (i/(double)nBins)*(binMax-binMin)+binMin << " " 
                      << bin[i] << std::endl;
          }
        writeStream.close();
        }

      // histogram2
      else if((*it).name == "histogram2")
        {
        std::cout << "Histogram" << std::endl;

        unsigned int nBins = (unsigned int)command.GetValueAsInt(*it,"nBins");
        double binMin = command.GetValueAsFloat(*it,"binMin");
        double binSize = command.GetValueAsFloat(*it,"binSIZE");
        double binMax = binMin + binSize*nBins;
        const char * filename = command.GetValueAsString(*it,"histOutputFile").c_str();

        itk::ImageRegionIteratorWithIndex< ImageType > it1(imIn, 
              imIn->GetLargestPossibleRegion());
        it1.GoToBegin();
        itk::Array<double> bin;
        bin.set_size(nBins);
        bin.Fill(0);
        while(!it1.IsAtEnd())
          {
          double tf = it1.Get();
          tf = (tf-binMin)/(binMax-binMin) * nBins;
          if(tf<nBins && tf>0)
            {
            bin[(int)tf]++;
            }
          ++it1;
          }
        std::ofstream writeStream;
        writeStream.open(filename, std::ios::binary | std::ios::out);
        for(unsigned int i=0; i<nBins; i++)
          {
          writeStream << (i/(double)nBins)*(binMax-binMin)+binMin << " " 
                      << bin[i] << std::endl;
          }
        writeStream.close();
        }

      // vessels
      else if((*it).name == "vessels")
        {
        std::cout << "Vessel Enhancement" << std::endl;

        double scaleMin = command.GetValueAsFloat(*it,"scaleMin");
        double scaleMax = command.GetValueAsFloat(*it,"scaleMax");
        double scaleStep = command.GetValueAsFloat(*it,"step");

        ImageFunctionType::Pointer imFunc = ImageFunctionType::New();
        imFunc->SetInputImage(imIn);

        ImageType::Pointer imIn2 = ImageType::New();
        imIn2->SetRegions(imIn->GetLargestPossibleRegion());
        imIn2->SetOrigin(imIn->GetOrigin());
        imIn2->SetSpacing(imIn->GetSpacing());
        imIn2->Allocate();

        itk::ImageRegionIteratorWithIndex< ImageType > it1(imIn, 
              imIn->GetLargestPossibleRegion());
        itk::ImageRegionIterator< ImageType > it2(imIn2, 
              imIn2->GetLargestPossibleRegion());

        double tf;
        double scale = scaleMin;
        std::cout << "   Processing scale " << scale << std::endl;
        it1.GoToBegin();
        it2.GoToBegin();
        while(!it1.IsAtEnd())
          {
          tf = imFunc->RidgenessAtContinuousIndex(it1.GetIndex(),
                                                          scale);
          it2.Set((PixelType)tf);
          ++it1;
          ++it2;
          }
        scale += scaleStep;
        if( scale <= scaleMax)
          {
          std::cout << "   Processing scale " << scale << std::endl;
          for(scale += scaleStep; scale <= scaleMax; scale += scaleStep)
            {
            it1.GoToBegin();
            it2.GoToBegin();
            while(!it1.IsAtEnd())
              {
              tf = imFunc->RidgenessAtContinuousIndex(it1.GetIndex(),
                                                              scale);
              if(tf > it2.Get())
                {
                it2.Set((PixelType)tf);
                }
              ++it1;
              ++it2;
              }
            }
          }
        it1.GoToBegin();
        it2.GoToBegin();
        while(!it1.IsAtEnd())
          {
          it1.Set(it2.Get());
          ++it1;
          ++it2;
          }
        }

      // CorrectionSlice
      else if((*it).name == "CorrectionSlice")
        {
        std::cout << "Correct intensity slice-by-slice" << std::endl;

        unsigned int numberOfBins = (unsigned int)command.GetValueAsInt(*it,"nBins");
        unsigned int numberOfMatchPoints = (unsigned int)command.GetValueAsInt(*it,"nMatchPoints");
        typedef itk::Image<PixelType, 2> ImageType2D;
        typedef itk::HistogramMatchingImageFilter< ImageType2D, ImageType2D >
            HistogramMatchFilterType;
        HistogramMatchFilterType::Pointer matchFilter;
        ImageType2D::Pointer im2DRef = ImageType2D::New();
        ImageType2D::Pointer im2DIn = ImageType2D::New();
        ImageType2D::SizeType size2D;
        size2D[0] = imIn->GetLargestPossibleRegion().GetSize()[0];
        size2D[1] = imIn->GetLargestPossibleRegion().GetSize()[1];
        im2DRef->SetRegions(size2D);
        im2DRef->Allocate();
        im2DIn->SetRegions(size2D);
        im2DIn->Allocate();
        itk::ImageRegionIterator< ImageType > it3D(imIn, 
              imIn->GetLargestPossibleRegion());
        itk::ImageRegionIterator< ImageType > it3DSliceStart(imIn, 
              imIn->GetLargestPossibleRegion());
        itk::ImageRegionIterator< ImageType2D > it2DRef(im2DRef, 
              im2DRef->GetLargestPossibleRegion());
        itk::ImageRegionIterator< ImageType2D > it2DIn(im2DIn, 
              im2DIn->GetLargestPossibleRegion());
        unsigned int z, y, x;
        it3D.GoToBegin();
        for(z=0; z<3 && z<imIn->GetLargestPossibleRegion().GetSize()[2]; z++)
          {
          it2DRef.GoToBegin();
          for(y=0; y<imIn->GetLargestPossibleRegion().GetSize()[1]; y++)
            {
            for(x=0; x<imIn->GetLargestPossibleRegion().GetSize()[0]; x++)
              {
              it2DRef.Set(it3D.Get());
              ++it2DRef;
              ++it3D;
              }
            }
          }
        for(; z<imIn->GetLargestPossibleRegion().GetSize()[2]; z++)
          {
          it2DIn.GoToBegin();
          it3DSliceStart = it3D;
          for(y=0; y<imIn->GetLargestPossibleRegion().GetSize()[1]; y++)
            {
            for(x=0; x<imIn->GetLargestPossibleRegion().GetSize()[0]; x++)
              {
              it2DIn.Set(it3D.Get());
              ++it2DIn;
              ++it3D;
              }
            }
          matchFilter = HistogramMatchFilterType::New();
          matchFilter->SetReferenceImage(im2DRef);
          matchFilter->SetInput(im2DIn);
          matchFilter->SetNumberOfHistogramLevels(numberOfBins);
          matchFilter->SetNumberOfMatchPoints(numberOfMatchPoints);
          matchFilter->Update();
          itk::ImageRegionIterator< ImageType2D > it2DOut(
                matchFilter->GetOutput(), 
                im2DIn->GetLargestPossibleRegion());
          it2DRef.GoToBegin();
          it2DOut.GoToBegin();
          it3D = it3DSliceStart;
          for(y=0; y<imIn->GetLargestPossibleRegion().GetSize()[1]; y++)
            {
            for(x=0; x<imIn->GetLargestPossibleRegion().GetSize()[0]; x++)
              {
              it2DRef.Set(it2DOut.Get());
              it3D.Set(it2DOut.Get());
              ++it2DRef;
              ++it2DOut;
              ++it3D;
              }
            }
          }
        }
      
      // Correction
      else if((*it).name == "Correction")
        {
        std::cout << "Correct intensity in the volume" << std::endl;

        unsigned int numberOfBins = (unsigned int)command.GetValueAsInt(*it,"nBins");
        unsigned int numberOfMatchPoints = (unsigned int)command.GetValueAsInt(*it,"nMatchPoints");
        typedef itk::HistogramMatchingImageFilter< ImageType, ImageType >
            HistogramMatchFilterType;
        VolumeReaderType::Pointer reader2 = VolumeReaderType::New();
        reader2->SetFileName( command.GetValueAsString(*it,"referenceVolume").c_str() );
        ImageType::Pointer imIn2;
        imIn2 = reader2->GetOutput();
        try
          {
          reader2->Update();
          }
        catch( ... )
          {
          std::cout << "Problems reading file format of inFile2." << std::endl;
          return 1;
          }
        HistogramMatchFilterType::Pointer matchFilter;
        matchFilter = HistogramMatchFilterType::New();
        matchFilter->SetReferenceImage(imIn2);
        matchFilter->SetInput(imIn);
        matchFilter->SetNumberOfHistogramLevels(numberOfBins);
        matchFilter->SetNumberOfMatchPoints(numberOfMatchPoints);
        matchFilter->Update();
        imIn = matchFilter->GetOutput();
        }
     
      // resize
      else if((*it).name == "resize")
        {
        std::cout << "Resampling." << std::endl;
        double factor = command.GetValueAsFloat(*it,"factor");

        ImageType::Pointer imSub2 = ImageType::New();
        imSub2->SetOrigin(imIn->GetOrigin());
        ImageType::SizeType size;
        ImageType::SpacingType spacing;
        if(factor != 0)
          {
          size[0] = (long unsigned int)
                    (imIn->GetLargestPossibleRegion().GetSize()[0]/factor);
          size[1] = (long unsigned int)
                    (imIn->GetLargestPossibleRegion().GetSize()[1]/factor);
          size[2] = (long unsigned int)
                    (imIn->GetLargestPossibleRegion().GetSize()[2]/factor);
          spacing[0] = imIn->GetSpacing()[0]*factor;
          spacing[1] = imIn->GetSpacing()[1]*factor;
          spacing[2] = imIn->GetSpacing()[2]*factor;
          }
        else
          {
          spacing[0] = imIn->GetSpacing()[0];
          spacing[1] = imIn->GetSpacing()[1];
          spacing[2] = imIn->GetSpacing()[2];
          double meanSpacing = ((spacing[0]+spacing[1])/2 + spacing[2])/2;
          factor = meanSpacing/spacing[0];
          size[0] = (long unsigned int)
                    (imIn->GetLargestPossibleRegion().GetSize()[0]/factor);
          factor = meanSpacing/spacing[1];
          size[1] = (long unsigned int)
                    (imIn->GetLargestPossibleRegion().GetSize()[1]/factor);
          factor = meanSpacing/spacing[2];
          size[2] = (long unsigned int)
                    (imIn->GetLargestPossibleRegion().GetSize()[2]/factor);
          spacing[0] = meanSpacing;
          spacing[1] = meanSpacing;
          spacing[2] = meanSpacing;
          }
        imSub2->SetRegions(size);
        imSub2->SetSpacing(spacing);
        imSub2->Allocate();

        imIn = ResampleImage( imIn, imSub2 );
        }

     // resize2
     else if((*it).name == "resize2")
        {
        std::cout << "Resampling" << std::endl;
        VolumeReaderType::Pointer reader2 = VolumeReaderType::New();
        reader2->SetFileName( command.GetValueAsString(*it,"inFile2").c_str() );
        ImageType::Pointer imIn2;
        imIn2 = reader2->GetOutput();
        try
          {
          reader2->Update();
          }
        catch( ... )
          {
          std::cout << "Problems reading file format of inFile2." << std::endl;
          return 1;
          }
        imIn = ResampleImage( imIn, imIn2 );
        }

      // segment
      else if((*it).name == "segment")
        {
        std::cout << "Segmenting" << std::endl;

        int mode = command.GetValueAsInt(*it,"mode");

        float threshLow = command.GetValueAsFloat(*it,"threshLow");
        float threshHigh = command.GetValueAsFloat(*it,"threshHigh");
        float labelValue = command.GetValueAsFloat(*it,"labelValue");

        float x = command.GetValueAsFloat(*it,"x");
        float y = command.GetValueAsFloat(*it,"y");
        float z = command.GetValueAsFloat(*it,"z");

        typedef itk::ConnectedThresholdImageFilter<ImageType, ImageType>
                   FilterType;
        FilterType::Pointer filter = FilterType::New();
        
        ImageType::IndexType seed;
        seed[0] = (long int)x;
        seed[1] = (long int)y;
        seed[2] = (long int)z;

        filter->SetInput(imIn);
        filter->SetLower(threshLow);
        filter->SetUpper(threshHigh);
        filter->AddSeed(seed);
        filter->SetReplaceValue(labelValue);
        filter->Update();

        imIn = filter->GetOutput();
        }

      // offset
      else if((*it).name == "offset")
        {
        double offset[3];
        offset[0] = command.GetValueAsFloat(*it,"offsetX");
        offset[1] = command.GetValueAsFloat(*it,"offsetY");
        offset[2] = command.GetValueAsFloat(*it,"offsetZ");
        imIn->SetOrigin(offset);
        }
     
      // SetRandom
      else if((*it).name == "SetRandom")
        {
        unsigned int seed = (unsigned int)command.GetValueAsInt(*it,"seedValue");
        srand(seed);
        gaussGen->Initialize((int)seed);
        } // end -S
     
      // Voronoi
      else if((*it).name == "Voronoi")
        {
        unsigned int numberOfCentroids = (unsigned int)command.GetValueAsInt(*it,"numberOfCentroids");
        unsigned int numberOfIterations = (unsigned int)command.GetValueAsInt(*it,"numberOfIterations");
        unsigned int numberOfSamples = (unsigned int)command.GetValueAsInt(*it,"numberOfSamples");
        std::string filename = command.GetValueAsString(*it,"centroidOutputFile");
        typedef itk::CVTImageFilter<ImageType, ImageType> FilterType;
        FilterType::Pointer filter = FilterType::New();
        filter->SetInput(imIn);
        filter->SetNumberOfSamples(numberOfSamples);
        filter->SetNumberOfCentroids(numberOfCentroids);
        filter->SetNumberOfIterations(numberOfIterations);
        filter->SetNumberOfSamplesPerBatch(numberOfIterations);
        filter->Update();

        std::ofstream writeStream;
        writeStream.open(filename.c_str(), std::ios::binary | std::ios::out);
        writeStream << numberOfCentroids << std::endl;
        for(unsigned int i=0; i<numberOfCentroids; i++)
          {
          for(unsigned int j = 0; j<3; j++)
            {
            writeStream << (*(filter->GetCentroids()))[i][j];
            if(j<2)
              {
              writeStream << " ";
              }
            }
          writeStream << std::endl;
          }
        writeStream.close();

        imIn = filter->GetOutput();
        ImageType::SizeType size = imIn->GetLargestPossibleRegion().GetSize();

        filename = filename + ".mat";

        vnl_matrix<int> aMat(numberOfCentroids, numberOfCentroids);
        aMat.fill(0);

        itk::Index<3> indx;
        itk::Index<3> indx2;
        itk::Index<3> indx3;
        indx.Fill(0);
        bool done = false;
        bool invalid = false;
        bool done2 = false;
        int c, n;
        while(!done)
          {
          c = (int)(imIn->GetPixel(indx)-1);
          indx2.Fill(0);
          indx2[0] = 1;
          invalid = false;
          done2 = false;
          while(!done2)
            {
            invalid = false;
            for(int d=0; d<3; d++)
              {
              indx3[d] = indx[d] + indx2[d];
              if(indx3[d] >= (int)size[d])
                {
                invalid = true;
                break;
                }
              }
            if(!invalid)
              {
              n = (int)(imIn->GetPixel(indx3)-1);
              if(c != n)
                {
                aMat(c, n) = 1;
                aMat(n, c) = 1;
                }
              }
            int i=0;
            indx2[i]++;
            while(!done2 && indx2[i]>=2)
              {
              indx2[i] = 0;
              i++;
              if(i>2)
                {
                done2 = true;
                }
              else
                {
                indx2[i]++;
                }
              }
            }
          int i = 0;
          indx[i]++;
          while(!done && indx[i]>=size[i])
            {
            indx[i] = 0;
            i++;
            if(i>2)
              {
              done = true;
              }
            else
              {
              if(i == 2)
                {
                std::cout << "Computing adjacency of slice : " << indx[2] 
                          << std::endl;
                }
              indx[i]++;
              }
            }
          }

        writeStream.open(filename.c_str(), std::ios::binary | std::ios::out);
        writeStream << numberOfCentroids << std::endl;
        for(unsigned int i=0; i<numberOfCentroids; i++)
          {
          for(unsigned int j = 0; j<numberOfCentroids; j++)
            {
            writeStream << aMat(i,j);
            if(j<numberOfCentroids-1)
              {
              writeStream << " ";
              }
            }
          writeStream << std::endl;
          }
        writeStream.close();

        } // end -S

    it++;
    }

  return 0;
  }
