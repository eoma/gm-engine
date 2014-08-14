#pragma once

#include <memory>
#include <string>

namespace GM {
	namespace Framework {
		class BufferManager; typedef std::shared_ptr<BufferManager> BufferManagerPtr;
		class VaoManager; typedef std::shared_ptr<VaoManager> VaoManagerPtr;
		class Mesh; typedef std::shared_ptr<Mesh> MeshPtr;

		class IPrimitive {
		public:
			IPrimitive(const std::string &name) : name(name) {}

			const std::string &get_name() const { return name; }
			virtual MeshPtr create(const BufferManagerPtr &buffer_manager, const VaoManagerPtr &vao_manager) = 0;

		private:
			std::string name;
		};
		typedef std::shared_ptr<IPrimitive> IPrimitivePtr;
	}
}