/* Copyright (c) 2012-2017 Big Ladder Software LLC. All rights reserved.
* See the LICENSE file for additional terms and conditions. */

#ifndef FOUNDATION_HPP_
#define FOUNDATION_HPP_

#include "Mesher.hpp"
#include "Functions.hpp"
#include "Geometry.hpp"

namespace Kiva {

class LIBKIVA_EXPORT Material
{
public:
  Material();
  Material(double k, double rho, double cp);
  double conductivity;  // [W/m-K] conductivity (boost function of z, t?)
  double density;  // [kg/m3] density
  double specificHeat;  // [J/kg-K] specific heat
};

class Layer
{
public:

  Material material;
  double thickness;  // [m] thickness
};

class LIBKIVA_EXPORT InputBlock
{
public:
  InputBlock();

  double x; // [m] block X origin relative to wall interior
  double z; // [m] block Z origin relative to wall top
  double width; // [m] block width extending from block X origin outward
  double depth; // [m] block depth extending from block z origin downward
  Material material;
  Box box;

};

class LIBKIVA_EXPORT Wall
{
public:

  double interiorEmissivity;
  double exteriorEmissivity;
  double exteriorAbsorptivity;
  double heightAboveGrade;  // [m]
  double depthBelowSlab;  // [m]
  std::vector <Layer> layers;

  double totalWidth();
  double totalResistance();
};

class LIBKIVA_EXPORT Slab
{
public:

  double emissivity;
  std::vector <Layer> layers;

  double totalWidth();
  double totalResistance();
};

class Mesh
{
public:
  Mesh();
  double minCellDim;  // [m]
  double maxNearGrowthCoeff;
  double maxDepthGrowthCoeff;
  double maxInteriorGrowthCoeff;
  double maxExteriorGrowthCoeff;

};

class Block
{
public:

  Polygon polygon;
  double xMin, xMax, yMin, yMax, zMin, zMax;
  Material material;

  enum BlockType
  {
    SOLID,
    INTERIOR_AIR,
    EXTERIOR_AIR
  };

  BlockType blockType;

  void setSquarePolygon();

};

class Surface
{
public:

  enum SurfaceType
  {
    ST_SLAB_CORE,
    ST_SLAB_PERIM,
    ST_WALL_INT,
    ST_WALL_EXT,
    ST_WALL_TOP,
    ST_GRADE,
    ST_SYMMETRY,
    ST_SYMMETRY_AIR,
    ST_FAR_FIELD,
    ST_FAR_FIELD_AIR,
    ST_DEEP_GROUND,
    ST_TOP_AIR_INT,
    ST_TOP_AIR_EXT
  };

  Polygon polygon;
  double xMin, xMax, yMin, yMax, zMin, zMax;
  SurfaceType type;
  double emissivity, absorptivity, temperature;

  enum BoundaryConditionType
  {
    ZERO_FLUX,
    INTERIOR_FLUX,
    EXTERIOR_FLUX,
    CONSTANT_TEMPERATURE,
    INTERIOR_TEMPERATURE,
    EXTERIOR_TEMPERATURE
  };
  BoundaryConditionType boundaryConditionType;

  enum Orientation
  {
    X_POS,
    X_NEG,
    Y_POS,
    Y_NEG,
    Z_POS,
    Z_NEG
  };
  Orientation orientation;

  std::vector<std::tuple<std::size_t,std::size_t,std::size_t> > indices;

  double area;

  void setSquarePolygon();
};

class RangeType
{
public:

  typedef std::pair<double,double> Range;

  enum Type
  {
    MIN_INTERIOR,
    MID_INTERIOR,
    MIN_EXTERIOR,
    MAX_EXTERIOR,
    DEEP,
    NEAR
  };
  Type type;
  Range range;
};

class Ranges
{
public:
  std::vector<RangeType> ranges;

  bool isType(double position,RangeType::Type type);

};

class LIBKIVA_EXPORT Foundation
{
public:

  Foundation();
  // Inputs

  // Site
  double deepGroundDepth;  // [m]
  double farFieldWidth;  // [m] distance from outside of wall to the edge
               // of the domain
  double foundationDepth; // [m] below top of wall
  double orientation;  // [radians] from north

  double deepGroundTemperature;  // [K]
  enum DeepGroundBoundary
  {
    DGB_AUTO,
    DGB_CONSTANT_TEMPERATURE,
    DGB_ZERO_FLUX
  };

  DeepGroundBoundary deepGroundBoundary;

  double wallTopInteriorTemperature;
  double wallTopExteriorTemperature;
  enum WallTopBoundary
  {
    WTB_ZERO_FLUX,
    WTB_LINEAR_DT
  };

  WallTopBoundary wallTopBoundary;

  Material soil;
  double soilAbsorptivity;  // [frac]
  double soilEmissivity;  // [frac]
  double surfaceRoughness;  // [m]

  // Geometry
  enum CoordinateSystem
  {
    CS_CARTESIAN,
    CS_CYLINDRICAL
  };
  CoordinateSystem coordinateSystem;

  int numberOfDimensions;  // 2 or 3

  bool useSymmetry;

  enum ReductionStrategy
  {
    RS_AP,
    RS_RR,
    RS_CUSTOM,
    RS_BOUNDARY
  };
  ReductionStrategy reductionStrategy;

  bool twoParameters;
  double reductionLength1;
  double reductionLength2;

  double linearAreaMultiplier;

  Polygon polygon;
  bool isXSymm, isYSymm;
  std::vector<bool> isExposedPerimeter;
  double exposedFraction;
  bool useDetailedExposedPerimeter;

  double buildingHeight;
  std::vector<Polygon3> buildingSurfaces;

  // Constructions
  Wall wall;
  bool hasWall;
  Slab slab;
  bool hasSlab;
  std::vector<InputBlock> inputBlocks;

  double perimeterSurfaceWidth;
  bool hasPerimeterSurface;

  // Meshing
  Mesh mesh;


  // Simulation Control
  enum NumericalScheme
  {
    NS_ADE,
    NS_EXPLICIT,
    NS_ADI,
    NS_IMPLICIT,
    NS_CRANK_NICOLSON,
    NS_STEADY_STATE
  };

  NumericalScheme numericalScheme;

  double fADI;  // ADI modified f-factor

  double tolerance;
  int maxIterations;

  double interiorConvectiveCoefficient;
  double exteriorConvectiveCoefficient;
  enum ConvectionCalculationMethod
  {
    CCM_AUTO,
    CCM_CONSTANT_COEFFICIENT
  };

  ConvectionCalculationMethod convectionCalculationMethod;

  // Derived variables
  MeshData xMeshData;
  MeshData yMeshData;
  MeshData zMeshData;
  std::vector<Block> blocks;
  std::vector<Surface> surfaces;

  std::map<Surface::SurfaceType, double> surfaceAreas;
  std::map<Surface::SurfaceType, bool> hasSurface;
  double netArea;
  double netPerimeter;

  void createMeshData();
};

}

#endif /* FOUNDATION_HPP_ */
