#include <memory>
#include <glm/glm.hpp>
#include "partial-action/modify-winged-vertex.hpp"
#include "winged/edge.hpp"
#include "winged/vertex.hpp"
#include "winged/mesh.hpp"
#include "state.hpp"
#include "action/ids.hpp"

namespace {
  enum class Operation { Edge, WriteIndex, WriteNormal, Move };
};

struct PAModifyWVertex :: Impl {
  Operation                   operation;
  ActionIds                   operands;
  std::unique_ptr <glm::vec3> vec3;

  void edge (WingedVertex& vertex, WingedEdge* e) {
    this->operation = Operation::Edge;
    this->operands.setVertex (0, &vertex);
    this->operands.setEdge   (0, vertex.edge ());
    vertex.edge              (e);
  }

  void writeIndex (WingedMesh& mesh, WingedVertex& vertex, unsigned int indexNumber) {
    this->operation = Operation::WriteIndex;
    this->operands.setVertex (0, &vertex);
    this->operands.setIndex  (1, indexNumber);
    this->operands.setIndex  (2, mesh.index (indexNumber));
    vertex.writeIndex        (mesh, indexNumber);
  }

  void writeNormal (WingedMesh& mesh, WingedVertex& vertex, const glm::vec3& normal) {
    this->operation = Operation::WriteNormal;
    this->operands.setVertex (0, &vertex);
    this->vec3.reset         (new glm::vec3 (vertex.savedNormal (mesh)));
    vertex.writeNormal       (mesh, normal);
  }

  void move (WingedMesh& mesh, WingedVertex& vertex, const glm::vec3& pos) {
    this->operation = Operation::Move;
    this->operands.setVertex (0, &vertex);
    this->vec3.reset         (new glm::vec3 (vertex.vector (mesh)));
    mesh.setVertex           (vertex.index (), pos);
  }

  void toggle (WingedMesh& mesh) { 
    WingedVertex& vertex = this->operands.getVertexRef (mesh,0);

    switch (this->operation) {
      case Operation::Edge: {
        WingedEdge* e = vertex.edge ();
        vertex.edge (this->operands.getEdge (mesh, 0)); 
        this->operands.setEdge (0,e);
        break;
      }
      case Operation::WriteIndex: {
        unsigned int indexNumber = this->operands.getIndexRef (1);
        unsigned int index       = mesh.index (indexNumber);
        vertex.writeIndex (mesh, indexNumber);
        this->operands.setIndex (2,index);
        break;
      }
      case Operation::WriteNormal: {
        glm::vec3 normal = mesh.normal (vertex.index ());
        mesh.setNormal   (vertex.index (), *this->vec3);
        this->vec3.reset (new glm::vec3 (normal));
        break;
      }
      case Operation::Move: {
        glm::vec3 pos = mesh.vector (vertex.index ());
        mesh.setVertex   (vertex.index (), *this->vec3);
        this->vec3.reset (new glm::vec3 (pos));
        break;
      }
      default: assert (false);
    }
  }

  void runUndo (WingedMesh& mesh) { this->toggle (mesh); }
  void runRedo (WingedMesh& mesh) { this->toggle (mesh); }
};

DELEGATE_BIG3 (PAModifyWVertex)

DELEGATE2 (void,PAModifyWVertex,edge       ,WingedVertex&,WingedEdge*)
DELEGATE3 (void,PAModifyWVertex,writeIndex ,WingedMesh&,WingedVertex&,unsigned int)
DELEGATE3 (void,PAModifyWVertex,writeNormal,WingedMesh&,WingedVertex&, const glm::vec3&)
DELEGATE3 (void,PAModifyWVertex,move,WingedMesh&,WingedVertex&,const glm::vec3&)
DELEGATE1 (void,PAModifyWVertex,runUndo,WingedMesh&)
DELEGATE1 (void,PAModifyWVertex,runRedo,WingedMesh&)
