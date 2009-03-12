/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    generateSpheres.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include <itkEllipseSpatialObject.h>
#include <itkSpatialObjectToImageFilter.h>
#include <itkImageFileWriter.h>

int main(int argc, char* argv[] )
{
  typedef itk::EllipseSpatialObject<3>   EllipseType;
  
  for(unsigned int r=5;r<20;r++)
    {
    std::cout << "Generating sphere with r=" << r << std::endl;
    EllipseType::Pointer ellipse = EllipseType::New();
    ellipse->SetRadius(r);

    // Center the circle in the image
    EllipseType::TransformType::OffsetType offset;
    offset.Fill(25);
    ellipse->GetObjectToParentTransform()->SetOffset(offset);
    ellipse->ComputeObjectToWorldTransform();

    typedef itk::Image<unsigned char,3> ImageType;

    typedef itk::SpatialObjectToImageFilter<EllipseType,ImageType> SpatialObjectToImageFilterType;
    SpatialObjectToImageFilterType::Pointer imageFilter = SpatialObjectToImageFilterType::New();
    imageFilter->SetInput(ellipse);
    imageFilter->SetInsideValue(255);
    imageFilter->SetOutsideValue(0);
    ImageType::SizeType size;
    size[0]=50;
    size[1]=50;
    size[2]=50;
    imageFilter->SetSize(size);

    //Update the filter
    imageFilter->Update();

    //
    typedef itk::ImageFileWriter<ImageType> ImageFileWriterType;
    ImageFileWriterType::Pointer writer = ImageFileWriterType::New();
    writer->SetUseCompression(true);
    char* filename = new char[255];
    sprintf(filename,"sphere_%d.mha",r);
    writer->SetFileName(filename);
    writer->SetInput(imageFilter->GetOutput());
    writer->Update();
    delete [] filename;
    }
  return 1;
}
