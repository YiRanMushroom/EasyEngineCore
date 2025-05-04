export module Easy.Math.Transforms;

export import Easy.Vendor.glm;

import Easy.Core.Basic;

namespace Easy {
    export template<typename T>
    concept IsTransform =
            requires(T t) {
                t.GetTransform();
            };

    export template<IsTransform... Transforms>
    struct Transform {
        std::tuple<Transforms...> PreviousTransforms{};

        template<IsTransform AnotherTransform>
        Transform<AnotherTransform, Transforms...> Then(auto &&... args) {
            return Transform<AnotherTransform, Transforms...>(std::tuple_cat(
                std::tuple(AnotherTransform(std::forward<decltype(args)>(args)...)),
                std::move(PreviousTransforms)));
        }

        decltype(auto) GetTransform() {
            return std::apply([](auto &&... transforms) {
                return (transforms.GetTransform() * ...);
            }, std::move(PreviousTransforms));
        }
    };

    export template<IsTransform T>
    Transform<T> MakeTransform(auto &&... args) {
        return Transform<T>(std::tuple(T(std::forward<decltype(args)>(args)...)));
    }

    export struct Translate {
        glm::vec3 Translation{};

        Translate(glm::vec3 translation) : Translation(translation) {}

        Translate(float x, float y, float z) : Translation(x, y, z) {}

        Translate(float x, float y) : Translation(x, y, 0.0f) {}

        [[nodiscard]] glm::mat4 GetTransform() const {
            return glm::translate(glm::mat4(1.0f), Translation);
        }
    };

    export struct Rotate {
        enum class Axis {
            X, Y, Z
        };

        float Angle{};
        Axis RotationAxis{Axis::Z};

        Rotate(float angle) : Angle(angle) {}
        Rotate(float angle, Axis axis) : Angle(angle), RotationAxis(axis) {}

        [[nodiscard]] glm::mat4 GetTransform() const {
            switch (RotationAxis) {
                case Axis::X:
                    return glm::rotate(glm::mat4(1.0f), glm::radians(Angle), glm::vec3(1.0f, 0.0f, 0.0f));
                case Axis::Y:
                    return glm::rotate(glm::mat4(1.0f), glm::radians(Angle), glm::vec3(0.0f, 1.0f, 0.0f));
                case Axis::Z:
                    return glm::rotate(glm::mat4(1.0f), glm::radians(Angle), glm::vec3(0.0f, 0.0f, 1.0f));
            }
            std::unreachable();
        }
    };

    export struct Scale {
        glm::vec3 ScaleFactor{};

        Scale(glm::vec3 scale) : ScaleFactor(scale) {}
        Scale(float x, float y, float z) : ScaleFactor(x, y, z) {}
        Scale(float x, float y) : ScaleFactor(x, y, 1.0f) {}

        [[nodiscard]] glm::mat4 GetTransform() const {
            return glm::scale(glm::mat4(1.0f), ScaleFactor);
        }
    };
}
