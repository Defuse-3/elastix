#ifndef __elxBSplineInterpolator_hxx
#define __elxBSplineInterpolator_hxx

#include "elxBSplineInterpolator.h"

namespace elastix
{
using namespace itk;


	/**
	 * ***************** BeforeEachResolution ***********************
	 */

	template <class TElastix>
		void BSplineInterpolator<TElastix>::
		BeforeEachResolution(void)
	{
		/** Get the current resolution level. */
		unsigned int level = 
			( this->m_Registration->GetAsITKBaseType() )->GetCurrentLevel();

    /** Set the SplineOrder, default value = 1. */
		unsigned int splineOrder = 1;
		
		/** Read the desired splineOrder from the parameterFile. */
		this->GetConfiguration()->ReadParameter( splineOrder,
      "BSplineInterpolationOrder", this->GetComponentLabel(), level, 0 );
    
		/** Set the splineOrder. */
		this->SetSplineOrder( splineOrder );
		 
	} // end BeforeEachResolution


} // end namespace elastix

#endif // end #ifndef __elxBSplineInterpolator_hxx

