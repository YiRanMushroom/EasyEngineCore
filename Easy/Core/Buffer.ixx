export module Easy.Core.Buffer;

import Easy.Core.Basic;

namespace Easy {
    export struct Buffer {
        uint8_t *Data = nullptr;
        uint64_t Size = 0;

        Buffer() = default;

        Buffer(uint64_t size) {
            Allocate(size);
        }

        Buffer(const Buffer &) = default;

        static Buffer Copy(Buffer other) {
            Buffer result(other.Size);
            std::memcpy(result.Data, other.Data, other.Size);
            return result;
        }

        void Allocate(uint64_t size) {
            Release();

            Data = new uint8_t[size];
            Size = size;
        }

        void Release() {
            delete[] Data;
            Data = nullptr;
            Size = 0;
        }

        template<typename T>
        T *As() {
            return (T *) Data;
        }

        operator bool() const {
            return (bool) Data;
        }
    };

    export struct ScopedBuffer {
        ScopedBuffer(Buffer buffer)
            : m_Buffer(buffer) {}

        ScopedBuffer(uint64_t size)
            : m_Buffer(size) {}

        ~ScopedBuffer() {
            m_Buffer.Release();
        }

        uint8_t *Data() { return m_Buffer.Data; }
        uint64_t Size() { return m_Buffer.Size; }

        template<typename T>
        T *As() {
            return m_Buffer.As<T>();
        }

        explicit operator bool() const { return m_Buffer; }

    private:
        Buffer m_Buffer;
    };
}
