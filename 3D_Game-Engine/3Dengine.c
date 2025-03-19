#include<stdio.h>
#include<stdlib.h>

typedef struct Vector {
    float x, y, z;
} Vector;

typedef struct Polygon {
    Vector verticies[3];
} Polygon;

typedef struct BSPNode {
    Polygon* polygons;
    int polygonCount;
    struct BSPNode* front;
    struct BSPNode* back;
} BSPNode;

BSPNode* createBSPNode() {
    BSPNode* node = (BSPNode*)malloc(sizeof(BSPNode));
    node->polygons = NULL;
    node->polygonCount = 0;
    node-> front = NULL;
    node-> back = NULL;
    return node;
}

int classifyPolygon(Polygon poly, Vector planeNormal, float planeD) {
    float dotProduct = poly.verticies[0].x * planeNormal.x +
                       poly.verticies[0].y * planeNormal.y +
                       poly.verticies[0].z * planeNormal.z;
    if (dotProduct > planeD) return 1;
    if (dotProduct < planeD) return -1;
    return 0;
}

void buildBSP(BSPNode* node, Polygon* polygons, int count) {
    if (count == 0) return;
    
    Vector planeNormal = polygons[0].verticies[0];
    float planeD = 0.0f;
    
    Polygon frontPolygons[count], backPolygons[count];
    int frontCount = 0, backCount = 0;
    
    for (int i = 0; i < count; i++) {
        int classification = classifyPolygon(polygons[i], planeNormal, planeD);
        if (classification == 1) frontPolygons[frontCount++] = polygons[i];
        else if (classification == -1) backPolygons[backCount++] = polygons[i];
        else node->polygons[node->polygonCount++] = polygons[i];
    }
    
    if (frontCount > 0) {
        node->front = createBSPNode();
        buildBSP(node->front, frontPolygons, frontCount);
    }
    
    if (backCount > 0) {
        node->back = createBSPNode();
        buildBSP(node->back, backPolygons, backCount);
    }
}

void renderBSP(BSPNode* node, Vector cameraPosition) {
    if (node == NULL) return;
    
    float dotProduct = node->polygons[0].verticies[0].x * cameraPosition.x +
                       node->polygons[0].verticies[0].y * cameraPosition.y +
                       node->polygons[0].verticies[0].z * cameraPosition.z;
    
    if (dotProduct > 0) {
        renderBSP(node->back, cameraPosition);
        for (int i = 0; i < node->polygonCount; i++)
            printf("Rendering polygon at (%f, %f, %f)\n",
                    node->polygons[i].verticies[0].x,
                    node->polygons[i].verticies[0].y,
                    node->polygons[i].verticies[0].z);
        renderBSP(node->front, cameraPosition);
    } else {
        renderBSP(node->front, cameraPosition);
        for (int i = 0; i < node->polygonCount; i++)
            printf("Rendering polygon at (%f, %f, %f)\n",
                    node->polygons[i].verticies[0].x,
                    node->polygons[i].verticies[0].y,
                    node->polygons[i].verticies[0].z);
        renderBSP(node->back, cameraPosition);
    }
}

int main() {
    Polygon polygons[3] = {
        {{ {1, 1, 1}, {2, 2, 2}, {3, 3, 3} }},
        {{ {4, 4, 4}, {5, 5, 5}, {6, 6, 6} }},
        {{ {7, 7, 7}, {8, 8, 8}, {9, 9, 9} }}
    };
    
    BSPNode* root = createBSPNode();
    buildBSP(root, polygons, 3);
    
    Vector cameraPosition = {0,0,0};
    printf("Render BSP Tree:\n");
    renderBSP(root, cameraPosition);
    
    return 0;
}