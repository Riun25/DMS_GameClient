#pragma once
#include "IManager.h"
#include "Entity.h"
#include "MeshRenderer.h"
#include "InputManager.h"
#include "Graphics/DearsGraphicsEngine.h"
#include "GameInfo.h"
#include "Button.h"
/// <summary>
/// UI 피킹등을 관리합니다.
/// </summary>
class UIManager
	: public IManager
{
public:
	UIManager() = delete;
	UIManager(entt::registry& _registry, InputManager* _pInputManager, std::shared_ptr<DearsGraphicsEngine> _pGraphicsEngine
		, GameInfo* _info)
		: mRegistry(_registry), mpInputManager(_pInputManager), mpGraphicsEngine(_pGraphicsEngine)
		, mIsAlreadyFontLoaded(false), maxNumLayer(1), mViewMatrix(Matrix()), mProjMatrix(Matrix()), mpInfo(_info) {}

	~UIManager() = default;

	virtual bool Initialize();

	virtual void FixedUpdate(float _fixedDTime);

	// 텍스처가 바뀌면 바꿔껴준다.
	virtual void Update(float _dTime);

	virtual void LateUpdate(float _dTime);

	// UI를 렌더한다.
	void Render(float _dTime);

	/// <summary>
	/// 추가한 UI에 대한 모든 컴포넌트를 삭제하고, Texture와 폰트 리소스도 삭제합니다.
	/// </summary>
	virtual void Finalize();

	/// 폰트 및 텍스트 ----------------------

	// [[[초기화에 설정]]]
	// [기본 경로, 파일 이름, 폰트 사이즈, 한글인가요?] 리소스 컨테이너에 폰트를 추가 (파일이름은 폰트 ID로 자동 설정)
	void AddFont(std::string _basePath, std::string _fileName, float _size, bool _isKorean);

	// [삭제 예정] 폰트를 모두 추가했으면 셋팅 완료를 호출함
	void FontSetFinish();

	// 텍스트를 3D 좌표로 쓸 경우 m_hasDepth를 true로 설정하고, m_position은 어떤 값이 있어도 무시됩니다. m_worldPosition의 값을 채워주세요.

	// 텍스트를 추가한다.
	void AddText(std::shared_ptr<Entity> _entity, std::u8string _text, std::string _fontName, Vector2 _pos, int _layer = 0, bool _isVisible = true, Vector4 _rgba = Vector4(0.0f, 0.0f, 0.0f, 1.0f));

	// int 숫자가 하나 변동하는 함수입니다. u8"소지금 : %d" -> 업데이트에서 직접 m_num1를 변경합니다.
	void AddTextwithInt(std::shared_ptr<Entity> _entity, std::u8string _text, std::string _fontName, Vector2 _pos, int _num, int _layer = 0, bool _isVisible = true, Vector4 _rgba = Vector4(0.0f, 0.0f, 0.0f, 1.0f));
	// int 숫자가 두 개 변동하는 함수입니다. u8"xz좌표: %d, %d" -> 업데이트에서 직접 m_num1, m_num2를 변경합니다.
	void AddTextwithInt2(std::shared_ptr<Entity> _entity, std::u8string _text, std::string _fontName, Vector2 _pos, int _num1, int _num2, int _layer = 0, bool _isVisible = true, Vector4 _rgba = Vector4(0.0f, 0.0f, 0.0f, 1.0f));
	// float 숫자가 하나 변동하는 함수입니다. u8"백분율 : %f" -> 업데이트에서 직접 m_num3를 변경합니다.
	void AddTextwithFloat(std::shared_ptr<Entity> _entity, std::u8string _text, std::string _fontName, Vector2 _pos, float _num, int _layer = 0, bool _isVisible = true, Vector4 _rgba = Vector4(0.0f, 0.0f, 0.0f, 1.0f));
	// float 숫자가 두 개 변동하는 함수입니다. u8"마우스 x좌표: %.2f \n마우스 y좌표: %.2f" -> 업데이트에서 직접 m_num3, m_num4를 변경합니다.
	void AddTextwithFloat2(std::shared_ptr<Entity> _entity, std::u8string _text, std::string _fontName, Vector2 _pos, float _num1, float _num2, int _layer = 0, bool _isVisible = true, Vector4 _rgba = Vector4(0.0f, 0.0f, 0.0f, 1.0f));

	// 텍스트를 제거한다.
	void RemoveText(std::shared_ptr<Entity> _entity);

	// 리소스 컨테이너 안의 폰트를 제거한다.
	void EraseFont(std::string _fontName);

	/// 2D 텍스처 ----------------------

	// [삭제 예정]리소스 컨테이너에 2D 텍스처를 추가한다.
	void AddTexture2D(std::string _basePath, std::string _fileName);

	// UI를 추가한다.
	void AddUI(std::shared_ptr<Entity> _entity, std::string _fileName, Vector2 _pos, Vector2 _size, int _layer = 0, Vector4 _rgba = Vector4(1.0f, 1.0f, 1.0f, 1.0f), bool _isVisible = true);

	// UI를 제거한다.
	void RemoveUI(std::shared_ptr<Entity> _entity);

	//리소스 컨테이너 안의 특정 텍스쳐를 지운다.
	void Erase_Textures(std::string _textureName);

	// 업데이트 시에 초기화에 넣었던 레이어보다 높은 값을 넣는다면 이 함수를 사용해서 최대 layerNum을 삽입할 것
	void SetMaxNumLayer(int _num);

	/// 버튼 ----------------------

	// Press UI 및 Text, 색상을 추가한다.
	void AddButtonPressUI(std::shared_ptr<Entity> _entity, std::string _UIImange, Vector2 _pressedPosition = Vector2(), Vector2 _pressedSize = Vector2());
	void AddButtonPressText(std::shared_ptr<Entity> _entity, std::string _font, std::u8string _text, Vector2 _pressedPosition = Vector2());
	void AddButtonPressColor(std::shared_ptr<Entity> _entity, Vector4 _PressedRGBA);

	// Hover UI 및 Text, 색상을 추가한다.
	void AddButtonHoveredUI(std::shared_ptr<Entity> _entity, std::string _UIImange, Vector2 _hoveringPosition = Vector2(), Vector2 _hoveringSize = Vector2());
	void AddButtonHoveredText(std::shared_ptr<Entity> _entity, std::string _font, std::u8string _text, Vector2 _hoveringPosition = Vector2());
	void AddButtonHoveredColor(std::shared_ptr<Entity> _entity, Vector4 _HoveredRGBA);

	// Press와 Hover 색상을 추가한다.
	void AddButtonAllColor(std::shared_ptr<Entity> _entity, Vector4 _PressedRGBA, Vector4 _HoveredRGBA);
	// Press와 Hover UI를 추가한다.[press이미지, 호버링 이미지, press위치와 사이즈, 호버링 위치와 사이즈]
	void AddButtonAllUI(std::shared_ptr<Entity> _entity, std::string _pressedUIImange, std::string _hoveringUIImange, Vector2 _pressedPosition = Vector2(), Vector2 _pressedSize = Vector2()
		, Vector2 _hoveringPosition = Vector2(), Vector2 _hoveringSize = Vector2());

	ButtonState GetButtonState(std::shared_ptr<Entity> _entity);

	// 버튼을 삭제한다.
	void RemoveButton(std::shared_ptr<Entity> _entity);

	// 사각형 ----------------------

	/// <summary>사각형을 추가한다.</summary>
	/// <param name="_entity">엔티티</param> <param name="_posXY">위치</param> <param name="_sizeWH">사이즈</param>
	/// <param name="_layer">레이어</param> <param name="_rgba">배경색</param> <param name="_thickness">두께 = 0.0f</param> <param name="_BorderRgba">테두리색 = Vector4()</param>
	void AddRect(std::shared_ptr<Entity> _entity, Vector2 _posXY, Vector2 _sizeWH, int _layer, Vector4 _rgba, float _thickness = 0.0f, Vector4 _BorderRgba = Vector4());

	/// <summary>직사각형을 삭제한다.</summary> <param name="_entity">엔티티</param>
	void RemoveRect(std::shared_ptr<Entity> _entity);

	// 타일  ----------------------
	// 현재 두께가 없는 걸로만 테스트 진행 예정
	/// <summary> 타일을 추가한다. 단, point는 시계방향으로 추가한다.</summary>
	/// <param name="_entity">엔티티</param> <param name="_posXY">위치</param> <param name="_sizeWH">사이즈</param>
	/// <param name="_layer">레이어</param> <param name="_rgba">배경색</param> <param name="_thickness">두께 = 0.0f</param> <param name="_BorderRgba">테두리색 = Vector4()</param>
	void AddTile(std::shared_ptr<Entity> _entity, Vector2 _posXY, Vector2 _sizeWH, int _layer, Vector4 _rgba, float _thickness = 0.0f, Vector4 _BorderRgba = Vector4());

	// 타일을 추가한 후 셋팅한다.
	void AddTileMap(std::shared_ptr<Entity> _entity, Vector2 _center, int _col, int _row);
	// 체력바 -------------------------

	/// <summary>2D 체력바를 추가한다.</summary>
	/// <param name="_entity">엔티티</param> <param name="_posXY">위치</param> <param name="_sizeWH">사이즈</param> <param name="_layer">레이어</param> 
		/// <param name="_backgroundRgba">배경색</param> <param name="_foregroundColor">체력색</param> <param name="_healthPercentage">체력비율 = 1.0f</param>
	void AddHPBar(std::shared_ptr<Entity> _entity, Vector2 _posXY, Vector2 _sizeWH, int _layer,
		Vector4 _backgroundRgba, Vector4 _foregroundColor, float _healthPercentage = 1.0f);

	/// <summary>3D 체력바를 추가한다.</summary>
	/// <param name="_entity">엔티티</param> <param name="_offset">유닛 위치로부터의 offset</param> <param name="_sizeWH">사이즈</param> <param name="_layer">레이어</param> 
	/// <param name="_backgroundRgba">배경색</param> <param name="_foregroundColor">체력색</param> <param name="_healthPercentage">체력비율 = 1.0f</param>
	void Add3DHPBar(std::shared_ptr<Entity> _entity, Vector3 _offset, Vector2 _sizeWH,
		int _layer, Vector4 _backgroundRgba, Vector4 _foregroundColor, float _healthPercentage = 1.0f);

	/// <summary> 체력바를 삭제한다. </summary> <param name="_entity">엔티티</param>
	void RemoveHPBar(std::shared_ptr<Entity> _entity);

	// 메세지 박스 --------------------

	/// <summary> 2D 공간에만 쓰이는 메세지 박스를 추가한다.(텍스트는 Vector4(1.0f)을 기본값으로 넣으세요) </summary>
	/// <param name="_entity">엔티티</param> <param name="_fileName">img 파일이름</param> <param name="_texturePos">img 위치</param> <param name="_size">img 사이즈</param> 
	/// <param name="_imgRgba">img RGBA(1.0f)</param> <param name="_text">텍스트</param> <param name="_textPos">텍스트 위치</param> <param name="_textScale">text 크기</param>
	/// <param name="_fontName">폰트이름</param> <param name="_textRgba">텍스트 rgba</param> <param name="_layer">레이어 = 0</param> <param name="_isVisible">보임 여부 = true</param>
	void AddMessageBox2D(std::shared_ptr<Entity> _entity, std::string _imgFileName, Vector2 _imgPos, Vector2 _size, Vector4 _imgRgba
		, std::u8string _text, Vector2 _textPos, float _textScale, std::string _fontName, Vector4 _textRgba, int _layer = 0, bool _isVisible = true);

	/// <summary> 3D 공간에만 쓰이는 메세지 박스를 추가한다.(텍스트는 Vector4(1.0f)을 기본값으로 넣으세요) </summary> 
	/// <param name="_entity">엔티티</param> <param name="_imgFileName">img 파일이름</param> <param name="_imgOffset">img 오프셋</param> <param name="_size">img 사이즈</param> 
	/// <param name="_imgRgba">img RGBA(1.0f)</param> <param name="_text">텍스트</param> <param name="_textOffset">text 오프셋</param> <param name="_textScale">text 크기</param>
	/// <param name="_fontName">폰트 이름</param> <param name="_textRgba">text RGBA</param> <param name="_worldPos">월드 pos</param> <param name="_worldPosOffset">월드 오프셋xy</param>
	///  <param name="_layer">레이어 = 0</param> <param name="_isVisible">보임 여부 = true</param>
	void AddMessageBox3D(std::shared_ptr<Entity> _entity, std::string _imgFileName, Vector2 _imgOffset, Vector2 _size, Vector4 _imgRgba
		, std::u8string _text, Vector2 _textOffset, float _textScale, std::string _fontName, Vector4 _textRgba, Vector3 _worldPos, Vector2 _worldPosOffset, int _layer = 0, bool _isVisible = true);

	/// <summary> 메세지 박스를 삭제한다. </summary> <param name="_entity">엔티티</param>
	void RemoveMessageBox2D(std::shared_ptr<Entity> _entity);
	void RemoveMessageBox3D(std::shared_ptr<Entity> _entity);

	void SetRenderDebugInfo(bool _fps, bool _camera);

private:
	entt::registry& mRegistry;
	std::shared_ptr<DearsGraphicsEngine> mpGraphicsEngine; // 그래픽스 엔진 객체
	InputManager* mpInputManager;
	bool mIsAlreadyFontLoaded; // 이미 폰트가 추가됐는지 확인해서, 추가 완료를 막는 플래그

	int maxNumLayer;  // 현재 사용된 최대 레이어 번호
	Matrix mViewMatrix; // 카메라의 뷰행렬
	Matrix mProjMatrix; // 카메라의 프로젝션 행렬

	GameInfo* mpInfo;

	bool m_rendFPS = true;
	bool m_rendCameraInfo = true;

private:

	// 카메라의 뷰행렬과 프로젝션 행렬이 달라졌을 때만 업데이트 한다.
	void UpdateViewNProjection(Camera* camera);

	// 월드 좌표를 NDC 좌표로 변환
	Vector2 ConvertWorldtoNDC(Vector3 _worldPos, const Matrix& viewMatrix, const Matrix& projMatrix);

	// 3D의 좌표를 NDC 좌표로 옮기기
	Vector2 ConvertNDCtoScreeen(Vector2 _NDCPos);

	// 버튼에 사용된 기존 이미지가 변경됐을 때, 이미지를 업데이트한다.
	void UpdateButton(Texture2D* _pTexture2D, Button* _pButton, Text* _pText);

	// 버튼의 상태를 체크한다.
	void CheckButtonState(Button* _pButton, bool _isHovered);

	void RenderUI(int _layerOrder);
	void RenderText(int _layerOrder);

	// 직사각형을 그린다.
	void RenderRectFilled(int _layerOrder);

	// 체력바를 그린다.
	void RenderHealthBar(int _layerOrder);

	// 타일을 그린다.
	void RenderTile(int _layerOrder);

	// 메세지 박스를 업데이트한다.
	void UpdateMessageBox(int _layerOrder);

	// 디버그 정보를 출력합니다.
	void RenderDebugInfo();
};