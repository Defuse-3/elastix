#ifndef __elxAdvancedMattesMutualInformationMetric_HXX__
#define __elxAdvancedMattesMutualInformationMetric_HXX__

#include "elxAdvancedMattesMutualInformationMetric.h"
#include "itkHardLimiterFunction.h"
#include "itkExponentialLimiterFunction.h"
#include <string>

namespace elastix
{
using namespace itk;

	/**
	 * ******************* Initialize ***********************
	 */

	template <class TElastix>
		void AdvancedMattesMutualInformationMetric<TElastix>
		::Initialize(void) throw (ExceptionObject)
	{
		TimerPointer timer = TimerType::New();
		timer->StartTimer();
		this->Superclass1::Initialize();
		timer->StopTimer();
		elxout << "Initialization of AdvancedMattesMutualInformation metric took: "
			<< static_cast<long>( timer->GetElapsedClockSec() * 1000 ) << " ms." << std::endl;

	} // end Initialize

	
	/**
	 * ***************** BeforeEachResolution ***********************
	 */

	template <class TElastix>
		void AdvancedMattesMutualInformationMetric<TElastix>
		::BeforeEachResolution(void)
	{
		/** \todo Adapt SecondOrderRegularisationMetric.
		 * Set alpha, which balances the similarity and deformation energy
		 * E_total = (1-alpha)*E_sim + alpha*E_def.
		 * 	metric->SetAlpha( config.GetAlpha(level) );	 */

		/** Get the current resolution level. */
		unsigned int level = 
			( this->m_Registration->GetAsITKBaseType() )->GetCurrentLevel();
		
		/** Get and set the number of histogram bins. */
		unsigned int numberOfHistogramBins = 32;
    this->GetConfiguration()->ReadParameter( numberOfHistogramBins,
      "NumberOfHistogramBins", this->GetComponentLabel(), level, 0 );
		this->SetNumberOfFixedHistogramBins( numberOfHistogramBins );
    this->SetNumberOfMovingHistogramBins( numberOfHistogramBins );

    unsigned int numberOfFixedHistogramBins = numberOfHistogramBins;
    unsigned int numberOfMovingHistogramBins = numberOfHistogramBins;
    this->GetConfiguration()->ReadParameter( numberOfFixedHistogramBins,
      "NumberOfFixedHistogramBins", this->GetComponentLabel(), level, 0 );
		this->GetConfiguration()->ReadParameter( numberOfMovingHistogramBins,
      "NumberOfMovingHistogramBins", this->GetComponentLabel(), level, 0 );
		this->SetNumberOfFixedHistogramBins( numberOfFixedHistogramBins );
    this->SetNumberOfMovingHistogramBins( numberOfMovingHistogramBins );

    /** Set whether a differentiable overlap should be used */
    std::string useDifferentiableOverlap = "true";
    this->GetConfiguration()->ReadParameter( useDifferentiableOverlap,
      "UseDifferentiableOverlap", this->GetComponentLabel(), level, 0 );
    if ( useDifferentiableOverlap == "false" )
    {
      this->SetUseDifferentiableOverlap(false);
    }
    else
    {
      this->SetUseDifferentiableOverlap(true);
    }

    /** Get and set the mask interpolation order */
		unsigned int movingMaskInterpolationOrder = 2;
		this->GetConfiguration()->ReadParameter( movingMaskInterpolationOrder,
      "MovingMaskInterpolationOrder", this->GetComponentLabel(), level, 0 );
		this->SetMovingImageMaskInterpolationOrder( movingMaskInterpolationOrder );

    /** Get and set whether the metric should check if enough samples map inside the moving image. */
    std::string checkNumberOfSamples = "true";
    this->GetConfiguration()->ReadParameter( checkNumberOfSamples, 
      "CheckNumberOfSamples", this->GetComponentLabel(), level, 0 );
    if ( checkNumberOfSamples == "false" )
    {
      this->SetRequiredRatioOfValidSamples(0.0);
    }
    else
    {
      this->SetRequiredRatioOfValidSamples(0.25);
    }

    /** Set limiters */
    typedef HardLimiterFunction< RealType, FixedImageDimension > FixedLimiterType;
    typedef ExponentialLimiterFunction< RealType, MovingImageDimension > MovingLimiterType;
    this->SetFixedImageLimiter( FixedLimiterType::New() );
    this->SetMovingImageLimiter( MovingLimiterType::New() );
    
    /** Get and set the number of histogram bins. */
		double fixedLimitRangeRatio = 0.01;
    double movingLimitRangeRatio = 0.01;
    this->GetConfiguration()->ReadParameter( fixedLimitRangeRatio,
      "FixedLimitRangeRatio", this->GetComponentLabel(), level, 0 );
    this->GetConfiguration()->ReadParameter( movingLimitRangeRatio,
      "MovingLimitRangeRatio", this->GetComponentLabel(), level, 0 );
		this->SetFixedLimitRangeRatio( fixedLimitRangeRatio );
    this->SetMovingLimitRangeRatio( movingLimitRangeRatio );

    /** Set bspline parzen kernel orders */
    unsigned int fixedKernelBSplineOrder = 0;
    unsigned int movingKernelBSplineOrder = 3;
    this->GetConfiguration()->ReadParameter( fixedKernelBSplineOrder,
      "FixedKernelBSplineOrder", this->GetComponentLabel(), level, 0 );
    this->GetConfiguration()->ReadParameter( movingKernelBSplineOrder, 
      "MovingKernelBSplineOrder", this->GetComponentLabel(), level, 0 );
		this->SetFixedKernelBSplineOrder( fixedKernelBSplineOrder );
    this->SetMovingKernelBSplineOrder( movingKernelBSplineOrder );

	} // end BeforeEachResolution
	
  
} // end namespace elastix


#endif // end #ifndef __elxAdvancedMattesMutualInformationMetric_HXX__
