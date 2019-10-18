//#include "Scene.h"
//void Scene::DrawScene()
//{
//	//Clear our backbuffer
//	float bgColor[4] = { (0.0f, 0.0f, 0.0f, 0.0f) };
//	d3d11DevCon->ClearRenderTargetView(renderTargetView, bgColor);
//
//	//Refresh the Depth/Stencil view
//	d3d11DevCon->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
//
//	//Set the WVP matrix and send it to the constant buffer in effect file
//	WVP = cube1World * camView * camProjection;
//	cbPerObj.WVP = XMMatrixTranspose(WVP);
//	d3d11DevCon->UpdateSubresource(cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0);
//	d3d11DevCon->VSSetConstantBuffers(0, 1, &cbPerObjectBuffer);
//
//	//Draw the first cube
//	d3d11DevCon->DrawIndexed(36, 0, 0);
//
//	WVP = cube2World * camView * camProjection;
//	cbPerObj.WVP = XMMatrixTranspose(WVP);
//	d3d11DevCon->UpdateSubresource(cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0);
//	d3d11DevCon->VSSetConstantBuffers(0, 1, &cbPerObjectBuffer);
//
//	//Draw the second cube
//	d3d11DevCon->DrawIndexed(36, 0, 0);
//
//	//Present the backbuffer to the screen
//	SwapChain->Present(0, 0);
//}