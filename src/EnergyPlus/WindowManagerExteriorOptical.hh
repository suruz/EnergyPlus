// EnergyPlus, Copyright (c) 1996-2016, The Board of Trustees of the University of Illinois and
// The Regents of the University of California, through Lawrence Berkeley National Laboratory
// (subject to receipt of any required approvals from the U.S. Dept. of Energy). All rights
// reserved.
//
// If you have questions about your rights to use or distribute this software, please contact
// Berkeley Lab's Innovation & Partnerships Office at IPO@lbl.gov.
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
//     similar designation, without Lawrence Berkeley National Laboratory's prior written consent.
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
//
// You are under no obligation whatsoever to provide any bug fixes, patches, or upgrades to the
// features, functionality or performance of the source code ("Enhancements") to anyone; however,
// if you choose to make your Enhancements available either publicly, or directly to Lawrence
// Berkeley National Laboratory, without imposing a separate written license agreement for such
// Enhancements, then you hereby grant the following license: a non-exclusive, royalty-free
// perpetual license to install, use, modify, prepare derivative works, incorporate into other
// computer software, distribute, and sublicense such enhancements or derivative works thereof,
// in binary and source code form.

#ifndef WindowManagerExteriorOptical_hh_INCLUDED
#define WindowManagerExteriorOptical_hh_INCLUDED


#include <map>
#include <tuple>
#include <memory>

// EnergyPlus Headers
#include <EnergyPlus.hh>

namespace FenestrationCommon {

  enum class WavelengthRange;
  enum class Property;
  enum class Side;

}

namespace SpectralAveraging {

  class CSpectralSampleData;
  class CSpectralSample;
  class CAngularSpectralSample;

}

namespace LayerOptics {

  class CBSDFLayer;
  class CCellDescription;
  class CMaterialBand;
  class CMaterialSingleBand;

}

namespace EnergyPlus {

  namespace WindowManager {

    class CWCEIntegrator;

    // Initialize window optical properties with Windows-CalcEngine routines
    void InitWCEOpticalData();

    std::shared_ptr< LayerOptics::CBSDFLayer > getBSDFLayer(
      const std::shared_ptr< EnergyPlus::DataHeatBalance::MaterialProperties >& t_Material,
      const FenestrationCommon::WavelengthRange t_Range );

    ///////////////////////////////////////////////////////////////////////////////
    //   CWCEMaterialFactory
    ///////////////////////////////////////////////////////////////////////////////
    class CWCEMaterialFactory {
    public:
      CWCEMaterialFactory( const std::shared_ptr< EnergyPlus::DataHeatBalance::MaterialProperties >& t_Material,
        const FenestrationCommon::WavelengthRange t_Range );

      std::shared_ptr< LayerOptics::CMaterialBand > getMaterial();

    protected:
      virtual void init() = 0;
      std::shared_ptr< LayerOptics::CMaterialBand > m_Material;
      std::shared_ptr< EnergyPlus::DataHeatBalance::MaterialProperties > m_MaterialProperties;
      FenestrationCommon::WavelengthRange m_Range;
      bool m_Initialized;

    };

    ///////////////////////////////////////////////////////////////////////////////
    //   CWCESpecularMaterialsFactory
    ///////////////////////////////////////////////////////////////////////////////
    class CWCESpecularMaterialsFactory : public CWCEMaterialFactory {
    public:
      CWCESpecularMaterialsFactory( const std::shared_ptr< EnergyPlus::DataHeatBalance::MaterialProperties >& t_Material,
        const FenestrationCommon::WavelengthRange t_Range );

    private:
      void init();

    };

    ///////////////////////////////////////////////////////////////////////////////
    //   CWCEMaterialDualBandFactory
    ///////////////////////////////////////////////////////////////////////////////
    class CWCEMaterialDualBandFactory : public CWCEMaterialFactory {
    // Common interface class for devices with materials defined over visible and solar range.
    // It is mainly intended from shading devices.
    public:
      CWCEMaterialDualBandFactory( const std::shared_ptr< EnergyPlus::DataHeatBalance::MaterialProperties >& t_Material,
        const FenestrationCommon::WavelengthRange t_Range );

    protected:
      void init();
      virtual std::shared_ptr< LayerOptics::CMaterialSingleBand > createVisibleRangeMaterial() = 0;
      virtual std::shared_ptr< LayerOptics::CMaterialSingleBand > createSolarRangeMaterial() = 0;

    };

    ///////////////////////////////////////////////////////////////////////////////
    //   CWCEVenetianBlindMaterialsFactory
    ///////////////////////////////////////////////////////////////////////////////
    class CWCEVenetianBlindMaterialsFactory : public CWCEMaterialDualBandFactory {
    public:
      CWCEVenetianBlindMaterialsFactory( const std::shared_ptr< EnergyPlus::DataHeatBalance::MaterialProperties >& t_Material,
        const FenestrationCommon::WavelengthRange t_Range );

    private:
      std::shared_ptr< LayerOptics::CMaterialSingleBand > createVisibleRangeMaterial();
      std::shared_ptr< LayerOptics::CMaterialSingleBand > createSolarRangeMaterial();

    };

    ///////////////////////////////////////////////////////////////////////////////
    //   CWCEScreenMaterialsFactory
    ///////////////////////////////////////////////////////////////////////////////
    class CWCEScreenMaterialsFactory : public CWCEMaterialDualBandFactory {
    public:
      CWCEScreenMaterialsFactory( const std::shared_ptr< EnergyPlus::DataHeatBalance::MaterialProperties >& t_Material,
        const FenestrationCommon::WavelengthRange t_Range );

    private:
      std::shared_ptr< LayerOptics::CMaterialSingleBand > createVisibleRangeMaterial();
      std::shared_ptr< LayerOptics::CMaterialSingleBand > createSolarRangeMaterial();

    };

    ///////////////////////////////////////////////////////////////////////////////
    //   CWCEDiffuseShadeMaterialsFactory
    ///////////////////////////////////////////////////////////////////////////////
    class CWCEDiffuseShadeMaterialsFactory : public CWCEMaterialDualBandFactory {
    public:
      CWCEDiffuseShadeMaterialsFactory( const std::shared_ptr< EnergyPlus::DataHeatBalance::MaterialProperties >& t_Material,
        const FenestrationCommon::WavelengthRange t_Range );

    private:
      std::shared_ptr< LayerOptics::CMaterialSingleBand > createVisibleRangeMaterial();
      std::shared_ptr< LayerOptics::CMaterialSingleBand > createSolarRangeMaterial();

    };

    ///////////////////////////////////////////////////////////////////////////////
    //   CWCEBSDFLayerFactory
    ///////////////////////////////////////////////////////////////////////////////
    class CWCEBSDFLayerFactory {
    public:
      CWCEBSDFLayerFactory( const std::shared_ptr< EnergyPlus::DataHeatBalance::MaterialProperties >& t_Material,
        const FenestrationCommon::WavelengthRange t_Range );

      std::shared_ptr< LayerOptics::CBSDFLayer > getBSDFLayer();

    protected:
      void init();

      virtual void createMaterialFactory() = 0;
      virtual std::shared_ptr< LayerOptics::CCellDescription > getCellDescription() = 0;

      const std::shared_ptr< EnergyPlus::DataHeatBalance::MaterialProperties > m_Material;
      const FenestrationCommon::WavelengthRange m_Range;
      bool m_Initialized;

      std::shared_ptr< CWCEMaterialFactory > m_MaterialFactory;
      std::shared_ptr< LayerOptics::CBSDFLayer > m_BSDFLayer;
    };

    ///////////////////////////////////////////////////////////////////////////////
    //   CWCESpecularLayerFactory
    ///////////////////////////////////////////////////////////////////////////////
    class CWCESpecularLayerFactory : public CWCEBSDFLayerFactory {
    public:
      CWCESpecularLayerFactory( const std::shared_ptr< EnergyPlus::DataHeatBalance::MaterialProperties >& t_Material,
        const FenestrationCommon::WavelengthRange t_Range );

    private:
      void createMaterialFactory();
      std::shared_ptr< LayerOptics::CCellDescription > getCellDescription();

    };

    ///////////////////////////////////////////////////////////////////////////////
    //   CWCEVenetianBlindLayerFactory
    ///////////////////////////////////////////////////////////////////////////////
    class CWCEVenetianBlindLayerFactory : public CWCEBSDFLayerFactory {
    public:
      CWCEVenetianBlindLayerFactory( const std::shared_ptr< EnergyPlus::DataHeatBalance::MaterialProperties >& t_Material,
        const FenestrationCommon::WavelengthRange t_Range );

    private:
      void createMaterialFactory();
      std::shared_ptr< LayerOptics::CCellDescription > getCellDescription();

    };

    ///////////////////////////////////////////////////////////////////////////////
    //   CWCEScreenLayerFactory
    ///////////////////////////////////////////////////////////////////////////////
    class CWCEScreenLayerFactory : public CWCEBSDFLayerFactory {
    public:
      CWCEScreenLayerFactory( const std::shared_ptr< EnergyPlus::DataHeatBalance::MaterialProperties >& t_Material,
        const FenestrationCommon::WavelengthRange t_Range );

    private:
      void createMaterialFactory();
      std::shared_ptr< LayerOptics::CCellDescription > getCellDescription();

    };

    ///////////////////////////////////////////////////////////////////////////////
    //   CWCEDiffuseShadeLayerFactory
    ///////////////////////////////////////////////////////////////////////////////
    class CWCEDiffuseShadeLayerFactory : public CWCEBSDFLayerFactory {
    public:
      CWCEDiffuseShadeLayerFactory( const std::shared_ptr< EnergyPlus::DataHeatBalance::MaterialProperties >& t_Material,
        const FenestrationCommon::WavelengthRange t_Range );

    private:
      void createMaterialFactory();
      std::shared_ptr< LayerOptics::CCellDescription > getCellDescription();

    };

  }

}

#endif