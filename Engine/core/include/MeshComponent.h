#ifndef MELONENGINE_MESHCOMPONENT_H
#define MELONENGINE_MESHCOMPONENT_H

#include <../assimp/include/assimp/Importer.hpp>
#include <../assimp/include/assimp/scene.h>
#include <../assimp/include/assimp/postprocess.h>

class MeshComponent {
public:
    MeshComponent();
    ~MeshComponent();
    bool LoadMesh(const char* fileName);
private:

};

#endif //MELONENGINE_MESHCOMPONENT_H
