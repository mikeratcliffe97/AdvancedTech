
#ifndef _GAME1_H_
#define _GAME1_H_

#include"DirectX11Game.h"
#include"GameSprite.h"
#include <Windows.h>
#include <d3d11.h>
#include <d3d10.h>
#include <xnamath.h>

class Game : public DirectX11Game
{
public:
	Game();
	virtual ~Game();

	bool Initialize();
	bool LoadContent();
	void UnloadContent();

	void Update(float dt);
	void Draw();

private:
	ID3D11VertexShader* SpriteVertexShader;
	ID3D11PixelShader* SpritePixelShader;

	ID3D11InputLayout* VertexBufferInputLayout;
	ID3D11Buffer* QuadVertexBuffer;

	ID3D11ShaderResourceView* SpriteTextureResourceView;
	ID3D11SamplerState* TextureSamplerState;
	ID3D11BlendState* TextureBlendState;

	GameSprite sprites_[2];
	ID3D11Buffer* WorldViewProjectionMatrixConstantBuffer;
	XMMATRIX ViewProjectionMatrix;

};

#endif
