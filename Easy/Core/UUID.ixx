export module Easy.Core.UUID;

import Easy.Core.Basic;

namespace Easy {
    export class UUID {
    public:
        UUID();

        UUID(uint64_t uuid);

        UUID(const UUID &) = default;

        operator uint64_t() const { return m_UUID; }

    private:
        uint64_t m_UUID;
    };
}

namespace std {

    export template<>
    struct hash<Easy::UUID> {
        std::size_t operator()(const Easy::UUID &uuid) const {
            return (uint64_t) uuid;
        }
    };
}

namespace Easy {
    static std::random_device s_RandomDevice;
    static std::mt19937_64 s_Engine(s_RandomDevice());
    static std::uniform_int_distribution<uint64_t> s_UniformDistribution;

    UUID::UUID()
        : m_UUID(s_UniformDistribution(s_Engine)) {}

    UUID::UUID(uint64_t uuid)
        : m_UUID(uuid) {}
}
