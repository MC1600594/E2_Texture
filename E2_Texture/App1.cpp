// Lab1.cpp
// Lab 2 example, textured quad
#include "App1.h"

App1::App1()
{
	mesh = nullptr;
	meshOther = nullptr;
	textureShader = nullptr;
	textureShader2 = nullptr;
}

void App1::init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in, bool VSYNC, bool FULL_SCREEN)
{
	// Call super/parent init function (required!)
	BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in, VSYNC, FULL_SCREEN);

	textureMgr->loadTexture(L"bunny", L"res/bunny.png");

	// Create Mesh object and shader object
	mesh = new TexturedQuad(renderer->getDevice(), renderer->getDeviceContext());

	textureMgr->loadTexture(L"brick", L"res/brick1.dds");
	meshOther = new TexturedQuad(renderer->getDevice(), renderer->getDeviceContext());

	textureShader = new TextureShader(renderer->getDevice(), hwnd);
	textureShader2 = new TextureShader(renderer->getDevice(), hwnd);

}


App1::~App1()
{
	// Run base application deconstructor
	BaseApplication::~BaseApplication();

	// Release the Direct3D object.
	if (mesh)
	{
		delete mesh;
		mesh = 0;
	}

	if (meshOther)
	{
		delete meshOther;
		meshOther = 0;
	}

	if (textureShader)
	{
		delete textureShader;
		textureShader = 0;
	}

	if (textureShader2)
	{
		delete textureShader2;
		textureShader2 = 0;
	}
}


bool App1::frame()
{
	bool result;

	result = BaseApplication::frame();
	if (!result)
	{
		return false;
	}
	
	// Render the graphics.
	result = render();
	if (!result)
	{
		return false;
	}

	return true;
}

bool App1::render()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

	// Clear the scene. (default blue colour)
	renderer->beginScene(0.39f, 0.58f, 0.92f, 1.0f);

	// Generate the view matrix based on the camera's position.
	camera->update();

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	worldMatrix = renderer->getWorldMatrix();
	viewMatrix = camera->getViewMatrix();
	projectionMatrix = renderer->getProjectionMatrix();

	// Send geometry data, set shader parameters, render object with shader

	worldMatrix = XMMatrixTranslation(2.0, 0.0, 0.0);
	textureShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"brick"));
	mesh->sendData(renderer->getDeviceContext());

	worldMatrix = XMMatrixTranslation(-2.0, 0.0, 0.0);
	textureShader2->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"bunny"));
	meshOther->sendData(renderer->getDeviceContext());

	textureShader->render(renderer->getDeviceContext(), mesh->getIndexCount());

	// Render GUI
	gui();

	// Swap the buffers
	renderer->endScene();

	return true;
}

void App1::gui()
{
	// Force turn off unnecessary shader stages.
	renderer->getDeviceContext()->GSSetShader(NULL, NULL, 0);
	renderer->getDeviceContext()->HSSetShader(NULL, NULL, 0);
	renderer->getDeviceContext()->DSSetShader(NULL, NULL, 0);

	// Build UI
	ImGui::Text("FPS: %.2f", timer->getFPS());
	ImGui::Checkbox("Wireframe mode", &wireframeToggle);

	// Render UI
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

