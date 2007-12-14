/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    ImageRegistrationApp.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __ImageRegistrationApp_h
#define __ImageRegistrationApp_h

#include "itkImage.h"


#include "LandmarkRegistrator.h"
#include "MomentRegistrator.h"
#include "RigidRegistrator.h"
#include "AffineRegistrator.h"
#include "DeformableRegistrator.h"
#include "itkAffineTransform.h"

#include "itkVersor.h"

// including all the type of transform that could possibly be used
// as templated type.

namespace itk
{

template< class TImage >
class ImageRegistrationApp : public Object
  {
  public:
    typedef ImageRegistrationApp Self;
    typedef Object Superclass;
    typedef SmartPointer<Self> Pointer;
    typedef SmartPointer<const Self> ConstPointer;
  
    itkNewMacro(Self);
    itkTypeMacro(ImageRegistrationApp, Object);
  
    typedef TImage                        ImageType;
    typedef typename ImageType::Pointer   ImagePointer;
    typedef typename TImage::PixelType    ImagePixelType ;
    typedef typename TImage::RegionType   RegionType ;
    typedef typename TImage::OffsetType   OffsetType ;
  
    typedef MomentRegistrator< TImage >               NoneRegistratorType;
    typedef typename NoneRegistratorType::TransformType 
                                                      NoneRegTransformType;

    typedef MomentRegistrator< TImage >               CenterRegistratorType;
    typedef typename CenterRegistratorType::TransformType 
                                                      CenterRegTransformType;

    typedef MomentRegistrator< TImage >               MassRegistratorType;
    typedef typename MassRegistratorType::TransformType  
                                                      MassRegTransformType;

    typedef MomentRegistrator< TImage >               MomentRegistratorType;
    typedef typename MomentRegistratorType::TransformType  
                                                      MomentRegTransformType;

    typedef LandmarkRegistrator                       LandmarkRegistratorType;
    typedef LandmarkRegistratorType::LandmarkType     LandmarkType;
    typedef LandmarkRegistratorType::LandmarkSetType  LandmarkSetType;
    typedef LandmarkRegistratorType::ParametersType   LandmarkParametersType;
    typedef LandmarkRegistratorType::ScalesType       LandmarkScalesType;
    typedef LandmarkRegistratorType::TransformType    LandmarkRegTransformType;
  
    typedef AffineTransform<double, 3>                LoadedRegTransformType;

    typedef RigidRegistrator< TImage >                    RigidRegistratorType;
    typedef typename RigidRegistratorType::ParametersType RigidParametersType;
    typedef typename RigidRegistratorType::ScalesType     RigidScalesType;
    typedef typename RigidRegistratorType::TransformType  RigidRegTransformType;
  
    typedef AffineRegistrator< TImage >                 AffineRegistratorType;
    typedef typename AffineRegistratorType::ParametersType 
                                                        AffineParametersType;
    typedef typename AffineRegistratorType::ScalesType  AffineScalesType;
    typedef typename AffineRegistratorType::TransformType 
                                                        AffineRegTransformType;
  
    typedef DeformableRegistrator< TImage >         DeformableRegistratorType;
    typedef typename DeformableRegistratorType::ParametersType  
                                                    DeformableParametersType;
    
    typedef AffineTransform<double, 3>              AffineTransformType;
    typedef itk::BSplineDeformableTransform<double, 3, 3> 
                                                    DeformableTransformType ;
  
    void SetOptimizerToOnePlusOne();

    void SetOptimizerToGradient();

    void SetOptimizerToOnePlusOnePlusGradient();

    void RegisterUsingNone();

    void RegisterUsingCenters();

    void RegisterUsingMass();

    void RegisterUsingMoments();

    void SetLoadedTransform(const LoadedRegTransformType & tfm);
    void SetLoadedDeformableTransform(const DeformableTransformType & tfm);
    void CompositeLoadedTransform(const LoadedRegTransformType & tfm);
    void RegisterUsingLoadedTransform();

    void RegisterUsingLandmarks(LandmarkSetType* fixedImageLandmarks,
                                LandmarkSetType* movingImageLandmarks) ;
  
    void RegisterUsingRigid() ;
  
    void RegisterUsingAffine() ;
    
    void RegisterUsingDeformable() ;
  
    void SetFixedImage(TImage* image);
    
    void SetMovingImage(TImage* image);
  
    itkGetObjectMacro(NoneRegTransform, NoneRegTransformType) ;
    itkGetObjectMacro(CenterRegTransform, CenterRegTransformType) ;
    itkGetObjectMacro(MassRegTransform, MassRegTransformType) ;
    itkGetObjectMacro(MomentRegTransform, MomentRegTransformType) ;
    itkGetObjectMacro(LandmarkRegTransform, LandmarkRegTransformType) ;
    itkGetObjectMacro(LoadedRegTransform, LoadedRegTransformType) ;
    itkGetObjectMacro(RigidRegTransform, RigidRegTransformType) ;
    itkGetObjectMacro(AffineRegTransform, AffineRegTransformType) ;
    itkGetObjectMacro(LandmarkAffineTransform, AffineTransformType) ;
    itkGetObjectMacro(RigidAffineTransform, AffineTransformType) ;
    itkGetObjectMacro(AffineAffineTransform, AffineTransformType) ;
    itkGetObjectMacro(FinalTransform, AffineTransformType);
    itkGetObjectMacro(DeformableRegTransform, DeformableTransformType);
    itkGetObjectMacro(FinalDeformableTransform, DeformableTransformType);
  
    itkSetMacro(LandmarkNumberOfIterations, unsigned int) ;
    itkGetConstMacro(LandmarkNumberOfIterations, unsigned int) ;
    itkSetMacro(LandmarkScales, LandmarkScalesType) ;
    itkGetConstMacro(LandmarkScales, LandmarkScalesType) ;
  
    itkSetMacro(RigidNumberOfIterations, unsigned int) ;
    itkGetConstMacro(RigidNumberOfIterations, unsigned int) ;
    itkSetMacro(RigidFixedImageStandardDeviation, double) ;
    itkGetConstMacro(RigidFixedImageStandardDeviation, double) ;
    itkSetMacro(RigidMovingImageStandardDeviation, double) ;
    itkGetConstMacro(RigidMovingImageStandardDeviation, double) ;
    itkSetMacro(RigidNumberOfSpatialSamples, unsigned int) ;
    itkGetConstMacro(RigidNumberOfSpatialSamples, unsigned int) ;
    itkSetMacro(RigidScales, RigidScalesType) ;
    itkGetConstMacro(RigidScales, RigidScalesType) ;
  
    itkSetMacro(AffineNumberOfIterations, unsigned int) ;
    itkGetConstMacro(AffineNumberOfIterations, unsigned int) ;
    itkSetMacro(AffineFixedImageStandardDeviation, double) ;
    itkGetConstMacro(AffineFixedImageStandardDeviation, double) ;
    itkSetMacro(AffineMovingImageStandardDeviation, double) ;
    itkGetConstMacro(AffineMovingImageStandardDeviation, double) ;
    itkSetMacro(AffineNumberOfSpatialSamples, unsigned int) ;
    itkGetConstMacro(AffineNumberOfSpatialSamples, unsigned int) ;
    itkSetMacro(AffineScales, AffineScalesType) ;
    itkGetConstMacro(AffineScales, AffineScalesType) ;
  
    itkSetMacro(DeformableNumberOfIterations, unsigned int);
    itkGetConstMacro(DeformableNumberOfIterations, unsigned int);
    itkSetMacro(DeformableNumberOfSpatialSamples, unsigned int);
    itkGetConstMacro(DeformableNumberOfSpatialSamples, unsigned int);
    itkSetMacro(DeformableNumberOfControlPoints, unsigned int);
    itkGetConstMacro(DeformableNumberOfControlPoints, unsigned int);
    
    itkSetMacro(FixedImageRegion, RegionType) ;
    itkGetConstMacro(FixedImageRegion, RegionType) ;
  
    ImagePointer GetNoneRegisteredMovingImage();
    ImagePointer GetCenterRegisteredMovingImage();
    ImagePointer GetMassRegisteredMovingImage();
    ImagePointer GetMomentRegisteredMovingImage();
    ImagePointer GetLandmarkRegisteredMovingImage();
    ImagePointer GetLoadedRegisteredMovingImage();
    ImagePointer GetRigidRegisteredMovingImage();
    ImagePointer GetAffineRegisteredMovingImage();
    ImagePointer GetFinalRegisteredMovingImage();

    itkGetMacro(RigidMetricValue, double);
    itkGetMacro(AffineMetricValue, double);
    itkGetMacro(DeformableMetricValue, double);
  
                                    
  protected:
    ImageRegistrationApp() ;
    virtual ~ImageRegistrationApp() ;

    void InitRigidParameters(RigidParametersType & p,
                             Point<double, 3> & center);
  
    void PrintUncaughtError() ;
  
    void PrintError(ExceptionObject &e) ;
  
  private:
    typedef enum { NONE, CENTER, MASS, MOMENT, LANDMARK, LOADED, DEFLOADED, RIGID, AFFINE, DEFORMABLE }
                 PriorRegistrationMethodType;

    typedef enum { ONEPLUSONE,
                   GRADIENT,
                   ONEPLUSONEPLUSGRADIENT,
                   LBFGS
                 } OptimizerMethodType;

    typename NoneRegTransformType::Pointer       m_NoneRegTransform ;
    typename AffineTransformType::Pointer        m_NoneAffineTransform ;
    typename CenterRegTransformType::Pointer     m_CenterRegTransform ;
    typename AffineTransformType::Pointer        m_CenterAffineTransform ;
    typename MassRegTransformType::Pointer       m_MassRegTransform ;
    typename AffineTransformType::Pointer        m_MassAffineTransform ;
    typename MomentRegTransformType::Pointer     m_MomentRegTransform ;
    typename AffineTransformType::Pointer        m_MomentAffineTransform ;
    typename LandmarkRegTransformType::Pointer   m_LandmarkRegTransform ;
    typename AffineTransformType::Pointer        m_LandmarkAffineTransform ;
    typename LoadedRegTransformType::Pointer     m_LoadedRegTransform ;
    typename AffineTransformType::Pointer        m_LoadedAffineTransform ;
    typename DeformableTransformType::Pointer    m_LoadedDeformableTransform ;
    typename RigidRegTransformType::Pointer      m_RigidRegTransform ;
    typename AffineTransformType::Pointer        m_RigidAffineTransform ;
    typename AffineRegTransformType::Pointer     m_AffineRegTransform ;
    typename AffineTransformType::Pointer        m_AffineAffineTransform ;
    typename DeformableTransformType::Pointer    m_DeformableRegTransform ;
    typename AffineTransformType::Pointer        m_FinalTransform;
    typename DeformableTransformType::Pointer    m_FinalDeformableTransform;
    
    DeformableParametersType            m_FinalParameters;
    
    ImagePointer                        m_FinalDeformableImageP;
  
    double m_RigidMetricValue;
    double m_AffineMetricValue;
    double m_DeformableMetricValue;

    TImage*             m_FixedImage ;
    TImage*             m_MovingImage ;
    
    RegionType          m_FixedImageRegion ;
  
    PriorRegistrationMethodType   m_PriorRegistrationMethod;

    OptimizerMethodType        m_OptimizerMethod;

    bool                m_NoneRegValid;

    bool                m_CenterRegValid;

    bool                m_MassRegValid;

    bool                m_MomentRegValid;

    unsigned int        m_LandmarkNumberOfIterations ;
    LandmarkScalesType  m_LandmarkScales ;
    bool                m_LandmarkRegValid;

    bool                m_LoadedRegValid;
    bool                m_LoadedDefValid;
  
    unsigned int        m_RigidNumberOfIterations ;
    double              m_RigidFixedImageStandardDeviation ;
    double              m_RigidMovingImageStandardDeviation ;
    unsigned int        m_RigidNumberOfSpatialSamples ;
    RigidScalesType     m_RigidScales ;
    bool                m_RigidRegValid;
  
    unsigned int        m_AffineNumberOfIterations ;
    double              m_AffineFixedImageStandardDeviation ;
    double              m_AffineMovingImageStandardDeviation ;
    unsigned int        m_AffineNumberOfSpatialSamples ;
    AffineScalesType    m_AffineScales ;
    bool                m_AffineRegValid;
    
    unsigned int        m_DeformableNumberOfIterations;
    unsigned int        m_DeformableNumberOfSpatialSamples;
    unsigned int        m_DeformableNumberOfControlPoints;
    bool                m_DeformableRegValid;
  
    ImagePointer        m_ResampleUsingTransform(
                                        AffineTransformType * transform,
                                        ImageType * input,
                                        ImageType * output);
    
    ImagePointer        m_ResampleUsingDeformableTransform(
                                        DeformableTransformType * transform,
                                        ImageType * input,
                                        ImageType * output);
  }; // end of class

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "ImageRegistrationApp.txx"
#endif

#endif //__ImageRegistrationApp_h

