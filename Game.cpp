#include"Game.h"
#include"GameSprite.h"


struct VertexOfPositionUV
{
	XMFLOAT3 Position;
	XMFLOAT2 tex0;
};


Game::Game() : SpriteVertexShader(0), SpritePixelShader(0),
VertexBufferInputLayout(0), QuadVertexBuffer(0),
SpriteTextureResourceView(0), TextureSamplerState(0),
WorldViewProjectionMatrixConstantBuffer(0), TextureBlendState(0)
{

}


Game::~Game()
{

}


bool Game::Initialize()
{
	bool Initialized = false;
	D3D11_INPUT_ELEMENT_DESC SpriteVertexLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	if (CreateVertexShader("Shaders\\BasicSpriteShader.fx", "VertexShaderCodeStart", "vs_4_0", &SpriteVertexShader, SpriteVertexLayout, ARRAYSIZE(SpriteVertexLayout), &VertexBufferInputLayout))
	{
		if (CreatePixelShader("Shaders\\BasicSpriteShader.fx", "PixelShaderCodeStart", "ps_4_0", &SpritePixelShader))
		{
			Initialized = true;
		}
	}

	return Initialized;
}


bool Game::LoadContent()
{
	HRESULT d3dResult;
	//A texture is the image from a picture file.
	if (SUCCEEDED(D3DX11CreateShaderResourceViewFromFile(GraphicsCardDevice, "Assets\\V.png", NULL, NULL, &SpriteTextureResourceView, NULL)))
	{

	}
	else
	{
		DXTRACE_MSG("Failed to load the sprite texture/image!");
		return false;
	}

	D3D11_SAMPLER_DESC TextureMappingDescription;
	ZeroMemory(&TextureMappingDescription, sizeof(TextureMappingDescription));
	TextureMappingDescription.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	TextureMappingDescription.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	TextureMappingDescription.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	TextureMappingDescription.ComparisonFunc = D3D11_COMPARISON_NEVER;
	TextureMappingDescription.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	TextureMappingDescription.MaxLOD = D3D11_FLOAT32_MAX;

	if (SUCCEEDED(GraphicsCardDevice->CreateSamplerState(&TextureMappingDescription, &TextureSamplerState)))
	{

	}
	else
	{
		DXTRACE_MSG("Failed to create color map sampler state!");
		return false;
	}

	ID3D11Resource* SpriteTexture1;
	SpriteTextureResourceView->GetResource(&SpriteTexture1);

	D3D11_TEXTURE2D_DESC SpriteTexture1Desc;
	((ID3D11Texture2D*)SpriteTexture1)->GetDesc(&SpriteTexture1Desc);
	SpriteTexture1->Release();

	float halfWidth = (float)SpriteTexture1Desc.Width / 2.0f;
	float halfHeight = (float)SpriteTexture1Desc.Height / 2.0f;


	VertexOfPositionUV SpriteVertices[] =
	{
		{ XMFLOAT3(halfWidth,  halfHeight, 1.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(halfWidth, -halfHeight, 1.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(-halfWidth, -halfHeight, 1.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(-halfWidth, -halfHeight, 1.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(-halfWidth,  halfHeight, 1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(halfWidth,  halfHeight, 1.0f), XMFLOAT2(1.0f, 0.0f) },
	};

	D3D11_BUFFER_DESC VertexDescription;
	ZeroMemory(&VertexDescription, sizeof(VertexDescription));
	VertexDescription.Usage = D3D11_USAGE_DEFAULT;
	VertexDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VertexDescription.ByteWidth = sizeof(VertexOfPositionUV) * 6;

	D3D11_SUBRESOURCE_DATA VertexResourceData;
	ZeroMemory(&VertexResourceData, sizeof(VertexResourceData));
	VertexResourceData.pSysMem = SpriteVertices;

	d3dResult = GraphicsCardDevice->CreateBuffer(&VertexDescription, &VertexResourceData, &QuadVertexBuffer);

	if (FAILED(d3dResult))
	{
		DXTRACE_MSG("Failed to create vertex buffer!");
		return false;
	}


	//Called a "Constant Buffer" because the data in the buffer never changes during a call through
	//the shader. The data may change every frame, or even during calls to draw multiple objects,
	//but it will not change at any point inside the shader.
	D3D11_BUFFER_DESC constDesc;
	ZeroMemory(&constDesc, sizeof(constDesc));
	constDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	//No other flags may be defined for Constant Buffers.
	constDesc.ByteWidth = sizeof(XMMATRIX);	//Must be a multiple of 16 bytes for all Constant Buffers.
	constDesc.Usage = D3D11_USAGE_DEFAULT;

	d3dResult = GraphicsCardDevice->CreateBuffer(&constDesc, NULL, &WorldViewProjectionMatrixConstantBuffer);

	if (FAILED(d3dResult))
	{
		return false;
	}


	XMFLOAT2 sprite1Pos(400.0f, 400.0f);
	sprites_[0].SetPosition(sprite1Pos);
	XMFLOAT2 sprite1Scale(1.0f, 1.0f);
	sprites_[0].SetScale(sprite1Scale);

	XMFLOAT2 sprite2Pos(1400.0f, 1600.0f);
	sprites_[1].SetPosition(sprite2Pos);
	XMFLOAT2 sprite2Scale(0.12f, 0.18f);
	sprites_[1].SetScale(sprite2Scale);

	XMMATRIX ViewMatrix = XMMatrixIdentity();
	XMMATRIX ProjectionMatrix = XMMatrixOrthographicOffCenterLH(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f);
	ViewProjectionMatrix = XMMatrixMultiply(ViewMatrix, ProjectionMatrix);


	D3D11_BLEND_DESC BlendDescription;
	ZeroMemory(&BlendDescription, sizeof(BlendDescription));
	BlendDescription.RenderTarget[0].BlendEnable = TRUE;
	BlendDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	BlendDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	BlendDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	BlendDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	BlendDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
	BlendDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	BlendDescription.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	float BlendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	unsigned int SampleMask = 0xFFFFFFFF;

	GraphicsCardDevice->CreateBlendState(&BlendDescription, &TextureBlendState);
	GraphicsCardDeviceContext->OMSetBlendState(TextureBlendState, BlendFactor, SampleMask);

	return true;
}


void Game::UnloadContent()
{
	if (TextureSamplerState) TextureSamplerState->Release();
	if (SpriteTextureResourceView) SpriteTextureResourceView->Release();
	if (SpriteVertexShader) SpriteVertexShader->Release();
	if (SpritePixelShader) SpritePixelShader->Release();
	if (VertexBufferInputLayout) VertexBufferInputLayout->Release();
	if (QuadVertexBuffer) QuadVertexBuffer->Release();
	if (WorldViewProjectionMatrixConstantBuffer) WorldViewProjectionMatrixConstantBuffer->Release();
	if (TextureBlendState) TextureBlendState->Release();

	TextureSamplerState = NULL;
	SpriteTextureResourceView = NULL;
	SpriteVertexShader = NULL;
	SpritePixelShader = NULL;
	VertexBufferInputLayout = NULL;
	QuadVertexBuffer = NULL;
	WorldViewProjectionMatrixConstantBuffer = NULL;
	TextureBlendState = NULL;
}


void Game::Update(float dt)
{
	// Nothing to update
}


void Game::Draw()
{
	GraphicsCardDeviceContext->ClearRenderTargetView(BackBuffer, RGBACornFlowerBlue);

	unsigned int VertexSizeInBytes = sizeof(VertexOfPositionUV);
	unsigned int Offset = 0;

	GraphicsCardDeviceContext->IASetInputLayout(VertexBufferInputLayout);
	GraphicsCardDeviceContext->IASetVertexBuffers(0, 1, &QuadVertexBuffer, &VertexSizeInBytes, &Offset);
	GraphicsCardDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	GraphicsCardDeviceContext->VSSetShader(SpriteVertexShader, 0, 0);
	GraphicsCardDeviceContext->PSSetShader(SpritePixelShader, 0, 0);
	GraphicsCardDeviceContext->PSSetShaderResources(0, 1, &SpriteTextureResourceView);
	GraphicsCardDeviceContext->PSSetSamplers(0, 1, &TextureSamplerState);

	for (int i = 0; i < 2; i++)
	{
		XMMATRIX SpritesWorldMatrix = sprites_[i].GetWorldMatrix();
		XMMATRIX SpritesWorldViewProjectionMatrix = XMMatrixMultiply(SpritesWorldMatrix, ViewProjectionMatrix);
		SpritesWorldViewProjectionMatrix = XMMatrixTranspose(SpritesWorldViewProjectionMatrix);

		GraphicsCardDeviceContext->UpdateSubresource(WorldViewProjectionMatrixConstantBuffer, 0, 0, &SpritesWorldViewProjectionMatrix, 0, 0);
		GraphicsCardDeviceContext->VSSetConstantBuffers(0, 1, &WorldViewProjectionMatrixConstantBuffer);

		GraphicsCardDeviceContext->Draw(6, 0);
	}

	SwapChain->Present(0, 0);

}
