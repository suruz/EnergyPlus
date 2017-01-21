// EnergyPlus, Copyright (c) 1996-2017, The Board of Trustees of the University of Illinois and
// The Regents of the University of California, through Lawrence Berkeley National Laboratory
// (subject to receipt of any required approvals from the U.S. Dept. of Energy). All rights
// reserved.
//
// NOTICE: This Software was developed under funding from the U.S. Department of Energy and the
// U.S. Government consequently retains certain rights. As such, the U.S. Government has been
// granted for itself and others acting on its behalf a paid-up, nonexclusive, irrevocable,
// worldwide license in the Software to reproduce, distribute copies to the public, prepare
// derivative works, and perform publicly and display publicly, and to permit others to do so.
//
// Redistribution and use in source and binary forms, with or without modification, are permitted
// provided that the following conditions are met:
//
// (1) Redistributions of source code must retain the above copyright notice, this list of
//     conditions and the following disclaimer.
//
// (2) Redistributions in binary form must reproduce the above copyright notice, this list of
//     conditions and the following disclaimer in the documentation and/or other materials
//     provided with the distribution.
//
// (3) Neither the name of the University of California, Lawrence Berkeley National Laboratory,
//     the University of Illinois, U.S. Dept. of Energy nor the names of its contributors may be
//     used to endorse or promote products derived from this software without specific prior
//     written permission.
//
// (4) Use of EnergyPlus(TM) Name. If Licensee (i) distributes the software in stand-alone form
//     without changes from the version obtained under this License, or (ii) Licensee makes a
//     reference solely to the software portion of its product, Licensee must refer to the
//     software as "EnergyPlus version X" software, where "X" is the version number Licensee
//     obtained under this License and may not use a different name for the software. Except as
//     specifically required in this Section (4), Licensee shall not use in a company name, a
//     product name, in advertising, publicity, or other promotional activities any name, trade
//     name, trademark, logo, or other designation of "EnergyPlus", "E+", "e+" or confusingly
//     similar designation, without the U.S. Department of Energy's prior written consent.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
// AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

// EnergyPlus::SortAndStringUtilities Unit Tests

// Google Test Headers
#include <gtest/gtest.h>

// EnergyPlus Headers
#include <EnergyPlus/General.hh>

#include "Fixtures/EnergyPlusFixture.hh"

using namespace EnergyPlus;

TEST_F( EnergyPlusFixture, General_ParseTime )
{
	int Hours;
	int Minutes;
	Real64 Seconds;
	{ // Time = 0
		General::ParseTime( 0, Hours, Minutes, Seconds );
		EXPECT_EQ( 0, Hours );
		EXPECT_EQ( 0, Minutes );
		EXPECT_DOUBLE_EQ( 0, Seconds );
	}
	{ // Time = 1
		General::ParseTime( 1, Hours, Minutes, Seconds );
		EXPECT_EQ( 0, Hours );
		EXPECT_EQ( 0, Minutes );
		EXPECT_DOUBLE_EQ( 1, Seconds );
	}
	{ // Time = 59
		General::ParseTime( 59, Hours, Minutes, Seconds );
		EXPECT_EQ( 0, Hours );
		EXPECT_EQ( 0, Minutes );
		EXPECT_DOUBLE_EQ( 59, Seconds );
	}
	{ // Time = 59.9
		General::ParseTime( 59.9, Hours, Minutes, Seconds );
		EXPECT_EQ( 0, Hours );
		EXPECT_EQ( 0, Minutes );
		EXPECT_DOUBLE_EQ( 59.9, Seconds );
	}
	{ // Time = 59.99
		General::ParseTime( 59.99, Hours, Minutes, Seconds );
		EXPECT_EQ( 0, Hours );
		EXPECT_EQ( 0, Minutes );
		EXPECT_DOUBLE_EQ( 59.99, Seconds );
	}
	{ // Time = 59.999
		General::ParseTime( 59.999, Hours, Minutes, Seconds );
		EXPECT_EQ( 0, Hours );
		EXPECT_EQ( 0, Minutes );
		EXPECT_DOUBLE_EQ( 59.999, Seconds );
	}
	{ // Time = 60
		General::ParseTime( 60, Hours, Minutes, Seconds );
		EXPECT_EQ( 0, Hours );
		EXPECT_EQ( 1, Minutes );
		EXPECT_DOUBLE_EQ( 0, Seconds );
	}
	{ // Time = 61
		General::ParseTime( 61, Hours, Minutes, Seconds );
		EXPECT_EQ( 0, Hours );
		EXPECT_EQ( 1, Minutes );
		EXPECT_DOUBLE_EQ( 1, Seconds );
	}
	{ // Time = 3599
		General::ParseTime( 3599, Hours, Minutes, Seconds );
		EXPECT_EQ( 0, Hours );
		EXPECT_EQ( 59, Minutes );
		EXPECT_DOUBLE_EQ( 59, Seconds );
	}
	{ // Time = 3600
		General::ParseTime( 3600, Hours, Minutes, Seconds );
		EXPECT_EQ( 1, Hours );
		EXPECT_EQ( 0, Minutes );
		EXPECT_DOUBLE_EQ( 0, Seconds );
	}
	{ // Time = 3601
		General::ParseTime( 3601, Hours, Minutes, Seconds );
		EXPECT_EQ( 1, Hours );
		EXPECT_EQ( 0, Minutes );
		EXPECT_DOUBLE_EQ( 1, Seconds );
	}
	{ // Time = 3661
		General::ParseTime( 3661, Hours, Minutes, Seconds );
		EXPECT_EQ( 1, Hours );
		EXPECT_EQ( 1, Minutes );
		EXPECT_DOUBLE_EQ( 1, Seconds );
	}
	{ // Time = 86399
		General::ParseTime( 86399, Hours, Minutes, Seconds );
		EXPECT_EQ( 23, Hours );
		EXPECT_EQ( 59, Minutes );
		EXPECT_DOUBLE_EQ( 59, Seconds );
	}
	{ // Time = 86400
		General::ParseTime( 86400, Hours, Minutes, Seconds );
		EXPECT_EQ( 24, Hours );
		EXPECT_EQ( 0, Minutes );
		EXPECT_DOUBLE_EQ( 0, Seconds );
	}
	{ // Time = 86401
		// Should probably be a failure
		General::ParseTime( 86401, Hours, Minutes, Seconds );
		EXPECT_EQ( 24, Hours );
		EXPECT_EQ( 0, Minutes );
		EXPECT_DOUBLE_EQ( 1, Seconds );
	}
	{ // Time = -1
		// Should probably be a failure
		General::ParseTime( -1, Hours, Minutes, Seconds );
		EXPECT_EQ( 0, Hours );
		EXPECT_EQ( 0, Minutes );
		EXPECT_DOUBLE_EQ( -1, Seconds );
	}
}

TEST_F( EnergyPlusFixture, General_CreateTimeString )
{
	{ // Time = 0
		EXPECT_EQ( "00:00:00.0", General::CreateTimeString( 0 ) );
	}
	{ // Time = 1
		EXPECT_EQ( "00:00:01.0", General::CreateTimeString( 1 ) );
	}
	{ // Time = 59
		EXPECT_EQ( "00:00:59.0", General::CreateTimeString( 59 ) );
	}
	{ // Time = 59.9
		EXPECT_EQ( "00:00:59.9", General::CreateTimeString( 59.9 ) );
	}
	{ // Time = 59.99
		EXPECT_EQ( "00:00:60.0", General::CreateTimeString( 59.99 ) );
	}
	{ // Time = 59.999
		EXPECT_EQ( "00:00:60.0", General::CreateTimeString( 59.999 ) );
	}
	{ // Time = 60
		EXPECT_EQ( "00:01:00.0", General::CreateTimeString( 60 ) );
	}
	{ // Time = 61
		EXPECT_EQ( "00:01:01.0", General::CreateTimeString( 61 ) );
	}
	{ // Time = 3600
		EXPECT_EQ( "01:00:00.0", General::CreateTimeString( 3600 ) );
	}
	{ // Time = 3599
		EXPECT_EQ( "00:59:59.0", General::CreateTimeString( 3599 ) );
	}
	{ // Time = 3601
		EXPECT_EQ( "01:00:01.0", General::CreateTimeString( 3601 ) );
	}
	{ // Time = 3661
		EXPECT_EQ( "01:01:01.0", General::CreateTimeString( 3661 ) );
	}
	{ // Time = 86399
		EXPECT_EQ( "23:59:59.0", General::CreateTimeString( 86399 ) );
	}
	{ // Time = 86400
		EXPECT_EQ( "24:00:00.0", General::CreateTimeString( 86400 ) );
	}
	{ // Time = 86401
		// Should probably be a failure
		EXPECT_EQ( "24:00:01.0", General::CreateTimeString( 86401 ) );
	}
	{ // Time = -1
		// Should probably be a failure
		EXPECT_EQ( "00:00:-1.0", General::CreateTimeString( -1 ) );
	}
}

TEST_F( EnergyPlusFixture, General_CreateTimeIntervalString )
{
	{ // Time = 0 - 1
		EXPECT_EQ( "00:00:00.0 - 00:00:01.0", General::CreateTimeIntervalString( 0, 1 ) );
	}
	{ // Time = 0 - 0
		EXPECT_EQ( "00:00:00.0 - 00:00:00.0", General::CreateTimeIntervalString( 0, 0 ) );
	}
	{ // Time = 1 - 0
		EXPECT_EQ( "00:00:01.0 - 00:00:00.0", General::CreateTimeIntervalString( 1, 0 ) );
	}
	{ // Time = 1 - 59
		EXPECT_EQ( "00:00:01.0 - 00:00:59.0", General::CreateTimeIntervalString( 1, 59 ) );
	}
	{ // Time = 59 - 59.9
		EXPECT_EQ( "00:00:59.0 - 00:00:59.9", General::CreateTimeIntervalString( 59, 59.9 ) );
	}
}
