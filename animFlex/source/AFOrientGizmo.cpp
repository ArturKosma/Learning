#include "AFOrientGizmo.h"

#include "AFApp.h"
#include "AFMath.h"
#include "AFOrientationGizmoUIComponent.h"
#include "AFOrientationGlyph.h"

void AFOrientGizmo::Tick(float deltaTime)
{
	glm::ivec2 dims = AFApp::GetWindowSize();
	const float counterScale = AFMath::MapRangeClamped(static_cast<float>(dims.y), 300.0f, 1000.0f, 2.0f, 1.0f); // #hack. Scale up the gizmo when window get smaller.

	const std::vector<std::shared_ptr<AFComponent>>& comps = GetComponents();
	for(auto& comp : comps)
	{
		// #hack. Scale orient glyph components to for smaller screens.
		// @todo Enable scaling whole UI.

		std::shared_ptr<AFOrientationGlyph> glyph = std::dynamic_pointer_cast<AFOrientationGlyph>(comp);
		if(glyph)
		{
			glyph->SetScale({ 0.5f * counterScale, 0.5f * counterScale });
		}

		std::shared_ptr<AFOrientationGizmoUIComponent> gizmo = std::dynamic_pointer_cast<AFOrientationGizmoUIComponent>(comp);
		{
			if(gizmo)
			{
				gizmo->SetScale({ 0.15f * counterScale, 0.15f * counterScale });
			}
		}
	}

	AFUI::Tick(deltaTime);
}
