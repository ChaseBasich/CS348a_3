#include <Eigen/Core>
#include <Eigen/Eigenvalues>
#include "curvature.h"

using namespace OpenMesh;
using namespace Eigen;

void computeCurvature(Mesh &mesh, OpenMesh::VPropHandleT<CurvatureInfo> &curvature) {

  for (Mesh::VertexIter v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ++v_it) {
    // WRITE CODE HERE TO COMPUTE THE CURVATURE AT THE CURRENT VERTEX ----------------------------------------------
    const Mesh::VertexHandle vh = (*v_it);
    const Vec3f normal = mesh.normal(vh);
    const Vector3d n(normal[0], normal[1], normal[2]); // example of converting to Eigen's vector class for easier math

    // In the end you need to fill in this struct

    CurvatureInfo info;
    info.curvatures[0] = 0;
    info.curvatures[1] = 0;
    info.directions[0] = Vec3f();
    info.directions[1] = Vec3f();

    mesh.property(curvature, vh) = info;
    // -------------------------------------------------------------------------------------------------------------
  }
}
