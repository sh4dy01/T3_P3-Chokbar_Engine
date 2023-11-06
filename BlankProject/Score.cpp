#include "BlankProject.h"
#include "Score.h"

Score::Score(const std::string& name, const std::string& texPath, float spacing, const DirectX::XMFLOAT3& position)
    : texturePath(texPath), digitSpacing(spacing), basePosition(position) {
    // Initialisation ici si nécessaire
}

Score::~Score() {
    ClearScore(); // Nettoyez vos objets ici
}

void Score::SetScore(unsigned int newScore) {
    ClearScore(); // Supprimez les anciens chiffres si nécessaire

    std::string scoreStr = std::to_string(newScore);

    DirectX::XMFLOAT3 currentPosition = basePosition;

    for (char digitChar : scoreStr) {
        int digitValue = digitChar - '0';
        AddDigit(digitValue, currentPosition);

        currentPosition.x += digitSpacing;
    }
}

float Score::CalculateUVOffsetForDigit(int digitValue) {
    const float unitOffset = 0.1f; // The height of each digit in UV space
    return digitValue * unitOffset;
}


void Score::AddDigit(int digitValue, const DirectX::XMFLOAT3& position) {
    auto* digit = new GameObject("Digit");
    auto* meshRenderer = new MeshRenderer();

    digit->AddComponent<MeshRenderer>(meshRenderer);
    meshRenderer->Init(MeshType::SQUARE, MaterialType::TEXTURE_TRANSPARENT);
    meshRenderer->RegisterTexture(Resource::Load<Texture>(texturePath));
    digit->transform->SetPosition(position.x, position.y, position.z);

    float uvOffsetY = CalculateUVOffsetForDigit(digitValue);
    meshRenderer->SetUVOffsetY(uvOffsetY);

    digitsObjects.push_back(digit);
}

void Score::ClearScore() {
    for (GameObject* digit : digitsObjects) {
        delete digit;
    }
    digitsObjects.clear();
}

void Score::Update() {

}

