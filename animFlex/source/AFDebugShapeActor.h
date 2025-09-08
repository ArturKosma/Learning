#pragma once
#include "AFActor.h"
#include "AFMesh.h"
#include "AFStructs.h"

class AFDebugShapeActor : public AFActor
{
public:
	void Tick(float deltaTime) override;

	void AssignDebugShape(EAFDebugShape shape, EAFColor color);

	void SetLifetime(float lifetime);
	float GetLifetime() const;

	void SetColor(AFSubMesh& subMesh, EAFColor color);

private:

	void FillVertices(EAFDebugShape shape, AFSubMesh& subMesh);

	float m_lifetime = 5.0f;
	std::shared_ptr<AFMesh> m_mesh = nullptr;
};
