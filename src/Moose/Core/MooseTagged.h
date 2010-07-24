/////////////////////////////////////////////////////////////////
#ifndef __MooseTagged_h__
#define __MooseTagged_h__
/////////////////////////////////////////////////////////////////
#include "MooseAPI.h"
/////////////////////////////////////////////////////////////////
namespace Moose
{
	namespace Core
	{
		typedef unsigned int TAG;  /* To be used in CTagged*/
        enum TagCompare
        {
          NOT_USED, 	///< Tag comparison is not used (default) (all tag values match).
          EQUAL,    	///< Matches only if tag is equal to given value.
          NOT_EQUAL, 	///< Matches only if tag is not equal to given value.
          AND, 		///< Matches if binary AND between object tag and given value yields non-zero value.
          NOT_AND 	///< Maches if binary AND between object tag and given value yields zero value.
        };
		/////////////////////////////////////////////////////////////////
		/// Class for taggable objects. Tag is an integer value which can be used in setting
		/// certain properties for a game object.
		class MOOSE_API CTagged
		{
		public:
			/// Comparision flags used when comparing tags in Octree CollectObjects();
			
		private:
			TAG                        m_Tag; ///< Tag value.
		public:
			////////////////////
			/// By default, tag is 0.
			CTagged() : m_Tag(0) {}
			///////////////////
			virtual ~CTagged () {};
			////////////////////
			/// Returns tag.
			/// \returns tag reference.
			const TAG & GetTag() const { return m_Tag;}
			////////////////////
			/// Sets tag.
			/// \param tag TAG to be set.
			void SetTag( TAG tag ) { m_Tag = tag; }
		};
	} // Core
} // Moose
/////////////////////////////////////////////////////////////////
#endif /* MOOSETAGGED_H_ */
