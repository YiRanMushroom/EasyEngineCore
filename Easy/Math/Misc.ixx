export module Easy.Math.Misc;

export import Easy.Vendor.glm;

namespace Easy {
    export void ConvertMouseToWorldPos(
        int width, int height,
        float mouseX, float mouseY,
        const glm::mat4 &viewMatrix,
        const glm::mat4 &projectionMatrix, glm::vec3 &worldPos) {
        float ndcX = (2.0f * mouseX) / width - 1.0f;
        float ndcY = 1.0f - (2.0f * mouseY) / height;

        glm::vec4 ndcNear = glm::vec4(ndcX, ndcY, -1.0f, 1.0f);
        glm::vec4 ndcFar = glm::vec4(ndcX, ndcY, 1.0f, 1.0f);

        glm::mat4 invVP = glm::inverse(projectionMatrix * viewMatrix);
        glm::vec4 worldNear = invVP * ndcNear;
        glm::vec4 worldFar = invVP * ndcFar;

        if (worldNear.w != 0.0f) worldNear /= worldNear.w;
        if (worldFar.w != 0.0f) worldFar /= worldFar.w;

        auto rayOrigin = glm::vec3(worldNear);
        glm::vec3 rayDirection = glm::normalize(glm::vec3(worldFar) - rayOrigin);

        if (rayDirection.z != 0.0f) {
            float t = -rayOrigin.z / rayDirection.z;
            worldPos = rayOrigin + t * rayDirection;
        } else {
            worldPos = glm::vec3(rayOrigin.x, rayOrigin.y, 0.0f);
        }
    }
}
