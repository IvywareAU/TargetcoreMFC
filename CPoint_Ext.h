// Copyright © 2014, 2026 Ivyware Pty Ltd, Khrustal & Mann
//              MELBOURNE, VICTORIA, AUSTRALIA, 3000
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
// implied. See the License for the specific language governing
// permissions and limitations under the License.
//
//
//  CFont extension prototypes and definitions
//

#pragma   once
#include "P2Pmsg.h"
#include "TargetCoreMFC.h"

//
//  P2Peer library Safe Pointer template
//  NOTES: Primarily used to recover memory from stack based pointers
//         when exceptions are thrown
//       : Designed for use within the P2Peer environment and not
//         to conflict with alternatives
//
template<class TPoint_t>
class TPoint
{
      TPoint() throw()
      {
	      m_valueX = 0;
	      m_valueY = 0;
      }
      TPoint ( TPoint_t valueX, TPoint_t valueY ) throw()
      {
	      m_valueX = valueX;
	      m_valueY = valueY;
      }
      TPoint ( const TPoint& rhs ) throw()
      {
	      *this = rhs;
      }

    void
      Offset ( TPoint_t offsetX, TPoint_t offsetY )
      {
	      m_valueX += offsetX;
	      m_valueY += offsetY;
      }
    void
      Offset ( const TPoint& oTPoint ) throw()
      {
	      m_valueX += oTPoint.m_valueX;
	      m_valueY += oTPoint.m_valueY;
      }
    void
      SetPoint ( int valueX, int valueY ) throw()
      {
	      m_valueX = valueX;
	      m_valueY = valueY;
      }
    BOOL
      operator == ( const TPoint& rhs ) const throw()
      {
	      return ( m_valueX == rhs.m_valueX && m_valueY == rhs.y);
      }
    void
      operator += ( const TPoint& rhs ) throw()
      {
	      m_valueX += rhs.m_valueX;
	      m_valueY += rhs.y;
      }
    void
      operator -= ( const TPoint& rhs ) throw()
      {
	      m_valueX -= rhs.m_valueX;
	      m_valueY -= rhs.y;
      }
    TPoint
      operator-() const throw()
      {
	      return TPoint(-m_valueX, -m_valueY);
      }

    TPoint
      operator + ( TPoint& rhs) const throw()
      {
	      return TPoint ( m_valueX + rhs.m_valueX, m_valueY + rhs.m_valueY);
      }
    // Attributes
    TPoint_t  m_valueX;
    TPoint_t  m_valueY;
};

typedef TPoint<double> DPoint;
typedef TPoint<float>  FPoint;