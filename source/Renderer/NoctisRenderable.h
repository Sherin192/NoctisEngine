#ifndef NOCTIS_RENDERABLE_H
#define NOCTIS_RENDERABLE_H


namespace noctis
{
	class Camera;
}
namespace noctis::rdr {
	class RenderDevice;

	template <typename Derived>
	struct IRenderable
	{
		void Render(std::shared_ptr<RenderDevice>& renderDevice, Camera& cam)
		{
			auto& child = static_cast<Derived&>(*this);
			child.Render(renderDevice, cam);
		}
	};
}
#endif //NOCTIS_RENDERABLE_H