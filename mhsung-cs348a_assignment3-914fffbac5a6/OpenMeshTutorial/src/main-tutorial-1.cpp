////
// CS348A
// OpenMesh Tutorial Code 1
// Oct. 2015
// Minhyuk Sung
//
// For understanding 'Halfedge Data Structure', please read:
// http://www.openmesh.org/media/Documentations/OpenMesh-Doc-Latest/a00016.html
//
// For understanding OpenMesh circulators, please read:
// http://www.openmesh.org/media/Documentations/OpenMesh-Doc-Latest/a00026.html
////


#include <cassert>
#include <iostream>
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/IO/Options.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include <OpenMesh/Tools/Utils/getopt.h>

using namespace std;
using namespace OpenMesh;


struct MyTraits : public OpenMesh::DefaultTraits
{
  HalfedgeAttributes(OpenMesh::Attributes::PrevHalfedge);
};

typedef OpenMesh::TriMesh_ArrayKernelT<MyTraits> Mesh;


void practice_1(const Mesh& mesh) {

  // Print vertex information.
  std::cout << "** Vertex Information **" << std::endl;
  for (Mesh::ConstVertexIter v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ++v_it) {
    const Mesh::VertexHandle vh(*v_it);
    const int vid = vh.idx();
    std::cout << "vid: [" << vid << "]" << std::endl;

    // Check whether it is on boundary.
    std::cout << "  - boundary: ";
    if (mesh.is_boundary(vh)) std::cout << "yes" << std::endl;
    else std::cout << "no" << std::endl;

    // Print vertex position and normal.
    const Vec3f p = mesh.point(vh);
    std::cout << "  - position: (" << p[0] << ", " << p[1] << ", " << p[2] << ")" << std::endl;
    const Vec3f n = mesh.normal(vh);
    std::cout << "  - normal: (" << n[0] << ", " << n[1] << ", " << n[2] << ")" << std::endl;

    // Print neighbor vertex indices.
    std::cout << "  - neighbors vertices: ";
    for (Mesh::ConstVertexVertexIter vv_it = mesh.cvv_begin(vh); vv_it != mesh.cvv_end(vh); ++vv_it) {
      const Mesh::VertexHandle n_vh(*vv_it);
      const int n_vid = n_vh.idx();
      std::cout << n_vid << " ";
    }
    std::cout << std::endl;

    // NOTE:
    // This is the same loop with the above.
    for (Mesh::ConstVertexVertexIter vv_it = mesh.cvv_iter(vh); vv_it.is_valid(); ++vv_it) {
      const Mesh::VertexHandle n_vh(*vv_it);
      // Do something.
    }

    // Print neighbor face indices.
    std::cout << "  - neighbors faces: ";
    for (Mesh::ConstVertexFaceIter vf_it = mesh.cvf_begin(vh); vf_it != mesh.cvf_end(vh); ++vf_it) {
      const Mesh::FaceHandle n_fh(*vf_it);
      const int n_fid = n_fh.idx();
      std::cout << n_fid << " ";
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
}

void practice_2(const Mesh& mesh) {

  // Print connectivity information.
  std::cout << "** Connectivity Information **" << std::endl;

  // For each vertex, print its connected edge information.
  for (Mesh::ConstVertexIter v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ++v_it) {
    const Mesh::VertexHandle vh(*v_it);
    const int vid = vh.idx();
    std::cout << "vid: [" << vid << "]" << std::endl;

    // Print connected edge information.
    std::cout << "  - connected edges: " << std::endl;
    for (Mesh::ConstVertexOHalfedgeIter vohe_it = mesh.cvoh_begin(vh); vohe_it != mesh.cvoh_end(vh); ++vohe_it) {
      const Mesh::HalfedgeHandle ohe(*vohe_it);

      // Get from and to vertices.
      const Mesh::VertexHandle from_vh = mesh.from_vertex_handle(ohe);
      const Mesh::VertexHandle to_vh = mesh.to_vertex_handle(ohe);
      assert(from_vh == vh);

      // Get the opposite halfedge.
      const Mesh::HalfedgeHandle ihe = mesh.opposite_halfedge_handle(ohe);

      // Get incident faces.
      const Mesh::FaceHandle n_fh_0 = mesh.face_handle(ohe);
      const Mesh::FaceHandle n_fh_1 = mesh.face_handle(ihe);

      // Compute edge length.
      const float length = mesh.calc_edge_length(ohe);

      std::cout << "    to_vid = " << to_vh.idx() << ", n_fid = (" <<
        n_fh_0.idx() << ", " << n_fh_1.idx() << "), length = " << length << std::endl;
    }
  }
  std::cout << std::endl;
}

void practice_3(const Mesh& mesh) {

  // Print face information.
  std::cout << "** Face Information **" << std::endl;
  for (Mesh::ConstFaceIter f_it = mesh.faces_begin(); f_it != mesh.faces_end(); ++f_it) {
    const Mesh::FaceHandle fh(*f_it);
    const int fid = fh.idx();
    std::cout << "fid: [" << fid << "]" << std::endl;

    // Print face centroid.
    const Vec3f c = mesh.calc_face_centroid(fh);
    std::cout << "  - centroid: (" << c[0] << ", " << c[1] << ", " << c[2] << ")" << std::endl;


    // Print face area.
    // You can make your own method for computing face area.
    // Here 'calc_sector_area()' function is used with an incident halfedge.
    const Mesh::EdgeHandle eh(*mesh.cfe_begin(fh));

    // Two halfedges.
    const Mesh::HalfedgeHandle heh_0 = mesh.halfedge_handle(eh, 0);
    const Mesh::HalfedgeHandle heh_1 = mesh.halfedge_handle(eh, 1);
    float area = 0;

    if (mesh.face_handle(heh_0) == fh) area = mesh.calc_sector_area(heh_0);
    else if (mesh.face_handle(heh_1) == fh) area = mesh.calc_sector_area(heh_1);
    else assert(false);
    std::cout << "  - area: " << area << std::endl;


    // Print neighbor vertex indices.
    std::cout << "  - neighbors vertices: ";
    for (Mesh::ConstFaceVertexIter fv_it = mesh.cfv_begin(fh); fv_it != mesh.cfv_end(fh); ++fv_it) {
      const Mesh::VertexHandle n_vh(*fv_it);
      const int n_vid = n_vh.idx();
      std::cout << n_vid << " ";
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
}


int main(int argc, char** argv) {
  Mesh mesh;
  IO::Options opt;
  opt += IO::Options::VertexNormal;
  opt += IO::Options::FaceNormal;

  // Add vertices.
  std::vector<Mesh::VertexHandle> vhandle(4);
  vhandle[0] = mesh.add_vertex(Mesh::Point(-1, -1,  1));
  vhandle[1] = mesh.add_vertex(Mesh::Point( 1, -1,  1));
  vhandle[2] = mesh.add_vertex(Mesh::Point( 1,  1,  1));
  vhandle[3] = mesh.add_vertex(Mesh::Point(-1,  1,  1));

  // Add faces.
  std::vector<Mesh::VertexHandle>  face_vhandles;

  face_vhandles.clear();
  face_vhandles.push_back(vhandle[1]);
  face_vhandles.push_back(vhandle[2]);
  face_vhandles.push_back(vhandle[0]);
  mesh.add_face(face_vhandles);

  face_vhandles.clear();
  face_vhandles.push_back(vhandle[3]);
  face_vhandles.push_back(vhandle[0]);
  face_vhandles.push_back(vhandle[2]);
  mesh.add_face(face_vhandles);

  // Request normals.
  mesh.request_face_normals();
  mesh.request_vertex_normals();
  mesh.update_normals();

  // Practice code.
  practice_1(mesh);
  practice_2(mesh);
  practice_3(mesh);

  return 0;
}
