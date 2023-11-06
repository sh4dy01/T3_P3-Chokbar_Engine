#include <string>
#include <vector>
#include <DirectXMath.h>

class Score : public GameObject
{
public:
    Score(const std::string& name, const std::string& texPath, float spacing, const DirectX::XMFLOAT3& position);
    ~Score();

    void SetScore(unsigned int newScore);
    void Update();
    float CalculateUVOffsetForDigit(int digitValue);

private:
    unsigned int score; // This will store the current score.
    std::vector<GameObject*> digitsObjects;
    std::string texturePath;
    float digitSpacing;
    DirectX::XMFLOAT3 basePosition;

    void AddDigit(int digitValue, const DirectX::XMFLOAT3& position);
    void ClearScore();
    void UpdateScoreDisplay(); // You might want to have this function to handle updating the display.
};
