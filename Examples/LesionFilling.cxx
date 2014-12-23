#include <algorithm>
#include <iomanip>
#include <iostream>
#include <ostream>
#include <sstream>
#include <vector>
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkSubtractImageFilter.h"
#include "itkBinaryDilateImageFilter.h"
#include "itkBinaryBallStructuringElement.h"
#include "antsUtilities.h"
#include "itkMaskImageFilter.h"
#include "itkConnectedComponentImageFilter.h"
#include "itkBinaryThresholdImageFilter.h"
#include "itkImageRegionIterator.h"

namespace ants
{
template <unsigned int Dimension>
int LesionFilling( int argc, char * argv[] )
{
  typedef unsigned char PixelType;
  typedef itk::Image<PixelType, Dimension> ImageType;
  typedef itk::ImageFileReader<ImageType>  ImageReaderType;

  typedef double           RealPixelType;  //  Operations
  typedef itk::ImageRegionIterator< LesionImageType> IteratorType;
  const int * Dimension = argv[1];
  const char * LesionMapFileName = argv[3];
  if (method == "-neighbourVoxels")
  {
         const char * T1FileName = argv[4]
  }
  else
  {
         const char * WMFileName = argv[4]
         const char * T1FileName = argv[5]
  }

  typename ImageReaderType::Pointer LesionReader = ImageReaderType::New();
  LesionReader->SetFileName( LesionMapFileName );
  LesionReader->Update();

  typename ImageReaderType::Pointer T1Reader = ImageReaderType::New();
  T1Reader->SetFileName( T1FileName); 
  T1Reader->Update();

  typedef itk::CastImageFilter< LesionImageType, RealImageType>
                                                         CastToRealFilterType;
  CastToRealFilterType::Pointer toReal = CastToRealFilterType::New();
  toReal->SetInput( LesionReader->GetOutput() );

  typedef itk::BinaryThresholdImageFilter <LesionImageType, LesionImageType>
                     BinaryThresholdImageFilterType;
  typedef itk::BinaryBallStructuringElement<
                               LesionImageType,
                               Dimension  >             StructuringElementType;
  typedef itk::BinaryDilateImageFilter<
                               LesionImageType,
                               LesionImageType,
                               StructuringElementType >  DilateFilterType;
  typedef itk::SubtractImageFilter <LesionImageType, LesionImageType>
                               SubtractImageFilterType;
  //finding connected components, we assume each component is one lesion
  typedef itk::ConnectedComponentImageFilter <LesionImageType, LesionImageType>
              ConnectedComponentImageFilterType;
  ConnectedComponentImageFilterType::Pointer connected =
              ConnectedComponentImageFilterType::New ();
  connected->SetInput( LesionReader->GetOutput() ) ;
  connected->Update();
  const int LesionNumber = connected->GetObjectCount() ;
  std::cout << "Number of lesions: " << LesionNumber << std::endl;
  std::vector<double> outervoxels;
  for ( int i = 1; i < LesionNumber; i++)
  {
     BinaryThresholdImageFilterType::Pointer thresholdFilter
                = BinaryThresholdImageFilterType::New();
     thresholdFilter->SetInput(connected->GetOutput());
     thresholdFilter->SetLowerThreshold( (double) i + 0.1);
     thresholdFilter->SetInsideValue  ( 1 );
     thresholdFilter->SetOutsideValue ( 0 );
     //Neighbouring voxel
     //filling lesions with the voxels surrounding them
     //first finding the edges of lesions
     //by subtracting dilated lesion map from lesion map itself
     DilateFilterType::Pointer binaryDilate = DilateFilterType::New();
     structuringElement.SetRadius( 1 );  // 3x3 structuring element
     structuringElement.CreateStructuringElement();
     binaryDilate->SetKernel( structuringElement );
     binaryDilate->SetInput( thresholdFilter->GetOutput() );
     binaryDilate->SetDilateValue( 1 );
     // subtract dilated image form non-dilated one
     SubtractImageFilterType::Pointer subtractFilter
                   = SubtractImageFilterType::New ();
     //output = image1 - image2
     subtractFilter->SetInput1(thresholdFilter->GetOutput() );
     subtractFilter->SetInput2(binaryDilate->GetOutput());
     subtractFilter->Update();
     //multiply the outer lesion mask with T1 to get only the neighbouring voxels
     typedef itk::MaskImageFilter< ImageType, ImageType > MaskFilterType;
     MaskFilterType::Pointer maskFilter = MaskFilterType::New();
     maskFilter->SetInput( mask t1 );
     maskFilter->SetMaskImage( subtractFilter->GetOutput() );
     //collecting non-zero voxels
     IteratorType it( maskFilter->GetOutput(),
                       maskFilter->GetOutput()->GetLargestPossibleRegion() );
     it.GoToBegin();
     /** Walk over the image. */
     while ( !it.IsAtEnd() )
       {
         if( it.Value() )
         {
           outervoxels.push_back ( it.Get() ); 
         }
         ++it;
       } // end while
     //calculating mean lesion intesity
     //Note: lesions should not be filled with values
     //less than their originial values, this is a
     //trick to exclude any CSF voxels in the outer mask (if any)
     MaskFilterType::Pointer maskFilterLesion = MaskFilterType::New();
     maskFilter->SetInput( 
     
     
  
  }

