#include "graphics.h"
#define M_PI 3.14159265358979323846


// Window Procedure Function
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}

// Initialize Direct3D
void initD3D(HWND hWnd, D3DContext& d3dContext)
{
    d3dContext.d3d = Direct3DCreate9(D3D_SDK_VERSION);

    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory(&d3dpp, sizeof(d3dpp));
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.hDeviceWindow = hWnd;
    d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
    d3dpp.BackBufferWidth = GetSystemMetrics(SM_CXSCREEN);
    d3dpp.BackBufferHeight = GetSystemMetrics(SM_CYSCREEN);

    d3dContext.d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
        D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &d3dContext.d3ddev);
}

D3DXVECTOR3 ToD3DXVECTOR3(const Vec3& vec) {
    return D3DXVECTOR3(vec.X, vec.Y, vec.Z);
}


void DrawRadar(IDirect3DDevice9* pDevice, const Player& player, const std::vector<Entity>& entities, float radarRadius, float radarCenterX, float radarCenterY)
{
    // Create a line object to be used for drawing shapes (like circles and lines)
    ID3DXLine* pLine;
    if (FAILED(D3DXCreateLine(pDevice, &pLine)))
        return; // If line creation fails, return early to avoid errors

    // Draw the grid with concentric circles and cross lines (no coordinates)
    const int gridDivisions = 4;  // Number of divisions for the grid
    float coordinateStep = radarRadius / gridDivisions;

    // Loop through each division to draw concentric circles and cross lines
    for (int i = 1; i <= gridDivisions; ++i)
    {
        // Draw concentric circles
        D3DXVECTOR2 circle[50];
        float currentRadius = coordinateStep * i;

        for (int j = 0; j < 50; j++)
        {
            float theta = (2.0f * D3DX_PI * float(j)) / 50.0f;
            circle[j].x = radarCenterX + currentRadius * cosf(theta);
            circle[j].y = radarCenterY + currentRadius * sinf(theta);
        }
        pLine->SetWidth(1.0f);
        pLine->Begin();
        pLine->Draw(circle, 50, D3DCOLOR_XRGB(50, 50, 50));  // Draw the grid circle in dark gray
        pLine->End();

        // Draw cross lines (no coordinate labels)
        for (int j = 0; j < 4; ++j)
        {
            float angle = j * (D3DX_PI / 2.0f);  // 90-degree increments for N, E, S, W
            D3DXVECTOR2 line[2];
            line[0] = D3DXVECTOR2(radarCenterX, radarCenterY);
            line[1] = D3DXVECTOR2(
                radarCenterX + currentRadius * cosf(angle),
                radarCenterY + currentRadius * sinf(angle)
            );
            pLine->Begin();
            pLine->Draw(line, 2, D3DCOLOR_XRGB(50, 50, 50));  // Draw the grid line in dark gray
            pLine->End();
        }
    }

    // Draw cardinal direction labels (N, E, S, W) around the radar
    const float textDistance = radarRadius + 15.0f;  // Distance from the center to place the labels
    DrawTextOnRadar(pDevice, "N", radarCenterX, radarCenterY - textDistance, D3DCOLOR_XRGB(255, 255, 255));  // North label
    DrawTextOnRadar(pDevice, "E", radarCenterX + textDistance, radarCenterY, D3DCOLOR_XRGB(255, 255, 255));  // East label
    DrawTextOnRadar(pDevice, "S", radarCenterX, radarCenterY + textDistance, D3DCOLOR_XRGB(255, 255, 255));  // South label
    DrawTextOnRadar(pDevice, "W", radarCenterX - textDistance, radarCenterY, D3DCOLOR_XRGB(255, 255, 255));  // West label


    /*
    
    
    // Draw a radar pulse effect
    static float pulseRadius = 0.0f;
    pulseRadius += 0.5f;  // Adjust speed of pulse expansion
    if (pulseRadius > radarRadius) pulseRadius = 0.0f;  // Reset pulse when it reaches radar edge

    D3DXVECTOR2 pulseCircle[50];
    for (int i = 0; i < 50; i++)
    {
        float theta = (2.0f * D3DX_PI * float(i)) / 50.0f;
        pulseCircle[i].x = radarCenterX + pulseRadius * cosf(theta);
        pulseCircle[i].y = radarCenterY + pulseRadius * sinf(theta);
    }

    pLine->SetWidth(2.0f);
    pLine->Begin();
    pLine->Draw(pulseCircle, 50, D3DCOLOR_ARGB(100, 255, 255, 255));  // Pulse in semi-transparent white
    pLine->End();

    */

    // Draw the player marker at the center of the radar
    D3DRECT playerRect = {
        (LONG)(radarCenterX - 3),  // Left coordinate of the rectangle
        (LONG)(radarCenterY - 3),  // Top coordinate of the rectangle
        (LONG)(radarCenterX + 3),  // Right coordinate of the rectangle
        (LONG)(radarCenterY + 3)   // Bottom coordinate of the rectangle
    };
    pDevice->Clear(1, &playerRect, D3DCLEAR_TARGET, D3DCOLOR_XRGB(255, 255, 255), 0.0f, 0);  // Draw the player in white

    // Draw the view direction as a line from the center
    float yawRadians = D3DXToRadian(player.Angles.Y);  // Convert player's yaw angle to radians
    D3DXVECTOR2 viewDir = { cosf(yawRadians), sinf(yawRadians) };  // Calculate the view direction vector
    D3DXVECTOR2 viewLineEnd = {
        radarCenterX + viewDir.x * radarRadius,  // End point of the view line (x)
        radarCenterY - viewDir.y * radarRadius   // End point of the view line (y)
    };

    // Draw the view direction line
    D3DXVECTOR2 viewLine[2] = { { radarCenterX, radarCenterY }, viewLineEnd };  // Line from center to the calculated end point
    pLine->SetWidth(1.0f);  // Set line width for the view direction
    pLine->Begin();
    pLine->Draw(viewLine, 2, D3DCOLOR_XRGB(255, 255, 0));  // Draw the view direction in yellow
    pLine->End();


    // Draw enemies relative to the player (without adjusting for the player's view angle)
    for (const auto& entity : entities)
    {
        if (entity.health <= 0 || entity.health > entity.maxHealth)
            continue;  // Skip dead entities

        D3DXVECTOR3 relativePos = ToD3DXVECTOR3(entity.Origin) - ToD3DXVECTOR3(player.Origin);  // Calculate the enemy's relative position to the player

        // Scale and transform the position to radar space
        float radarX = radarCenterX + (relativePos.x / 1.0f);  // Scale the x-coordinate for the radar
        float radarY = radarCenterY - (relativePos.z / 1.0f);  // Scale the y-coordinate for the radar

        // Calculate the distance to the enemy (convert from centimeters to meters)
        float distance = sqrtf(relativePos.x * relativePos.x + relativePos.y * relativePos.y + relativePos.z * relativePos.z);

        // Calculate the health ratio (current health divided by max health, converted to float)
        float healthRatio = static_cast<float>(entity.health) / static_cast<float>(entity.maxHealth);

        // Clamp the health ratio between 0 and 1 to avoid overflow or underflow
        healthRatio = min(1.0f, max(0.0f, healthRatio));

        // Interpolate color based on health (Green -> Red)
        int red = int(255 * (1.0f - healthRatio));  // More health = less red, low health = more red
        int green = int(255 * healthRatio);         // More health = more green, low health = less green
        D3DCOLOR enemyColor = D3DCOLOR_XRGB(red, green, 0);  // Create the color for the enemy

        // Only draw the enemy if within radar range
        if (fabs(radarX - radarCenterX) <= radarRadius && fabs(radarY - radarCenterY) <= radarRadius)
        {
            // Draw the enemy on the radar using the dynamically determined color based on health
            D3DRECT enemyRect = {
                (LONG)(radarX - 2),  // Left coordinate
                (LONG)(radarY - 2),  // Top coordinate
                (LONG)(radarX + 2),  // Right coordinate
                (LONG)(radarY + 2)   // Bottom coordinate
            };
            pDevice->Clear(1, &enemyRect, D3DCLEAR_TARGET, enemyColor, 0.0f, 0);  // Draw the enemy with the calculated color

            // Display the distance to the enemy above their position on the radar (in meters)
            char distanceText[16];
            sprintf_s(distanceText, "%.1fm", distance);  // Convert distance to string with "m" appended
            DrawTextOnRadar(pDevice, distanceText, radarX, radarY - 10, D3DCOLOR_XRGB(255, 255, 255));  // Draw the distance above the enemy dot
        }
    }

    // Draw a textured or stylized border around the radar (optional)
    // You can load a border texture and map it around the radar area

    // Release the line object to free resources
    pLine->Release();
}

// Helper function to draw text on the radar
void DrawTextOnRadar(IDirect3DDevice9* pDevice, const char* text, float x, float y, D3DCOLOR color)
{
    // Create a smaller font for the distance text
    ID3DXFont* pFont;
    D3DXCreateFont(pDevice, 10, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &pFont);

    // Define the rectangle area where the text will be drawn
    RECT fontRect;
    fontRect.left = (LONG)x - 10;    // Left coordinate of the rectangle
    fontRect.top = (LONG)y - 10;     // Top coordinate of the rectangle
    fontRect.right = (LONG)x + 10;   // Right coordinate of the rectangle
    fontRect.bottom = (LONG)y + 10;  // Bottom coordinate of the rectangle

    // Draw the text centered within the rectangle
    pFont->DrawTextA(NULL, text, -1, &fontRect, DT_CENTER | DT_NOCLIP, color);

    // Release the font object to free resources
    pFont->Release();
}


// Draw a Line using ID3DXLine
void DrawLine(IDirect3DDevice9* pDevice, float x1, float y1, float x2, float y2)
{
    ID3DXLine* pLine;
    if (FAILED(D3DXCreateLine(pDevice, &pLine)))
        return;

    D3DXVECTOR2 lineVertices[] = { D3DXVECTOR2(x1, y1), D3DXVECTOR2(x2, y2) };

    pLine->SetWidth(1.0f);  // Optional: set line width

    pLine->Begin();
    pLine->Draw(lineVertices, 2, D3DCOLOR_XRGB(255, 255, 255)); // Drawing the line in white
    pLine->End();

    pLine->Release();
}

// Draw a Line using ID3DXLine with NDC coordinates
void DrawLineNDC(IDirect3DDevice9* pDevice, float ndcX1, float ndcY1, float ndcX2, float ndcY2)
{
    if (!pDevice) return;

    // Retrieve the current viewport
    D3DVIEWPORT9 viewport;
    pDevice->GetViewport(&viewport);

    // Convert NDC (-1 to 1) to pixel coordinates
    float x1 = viewport.X + ((ndcX1 + 1.0f) * 0.5f * viewport.Width);
    float y1 = viewport.Y + ((1.0f - (ndcY1 + 1.0f)) * 0.5f * viewport.Height);

    float x2 = viewport.X + ((ndcX2 + 1.0f) * 0.5f * viewport.Width);
    float y2 = viewport.Y + ((1.0f - (ndcY2 + 1.0f)) * 0.5f * viewport.Height);

    // Create the line object
    ID3DXLine* pLine;
    if (FAILED(D3DXCreateLine(pDevice, &pLine)))
        return;

    D3DXVECTOR2 lineVertices[] = { D3DXVECTOR2(x1, y1), D3DXVECTOR2(x2, y2) };

    pLine->SetWidth(3.0f);  // Optional: set line width

    pLine->Begin();
    pLine->Draw(lineVertices, 2, D3DCOLOR_XRGB(255, 255, 255)); // Drawing the line in white
    pLine->End();

    pLine->Release();
}


// Render a Frame
void renderFrame(D3DContext& d3dContext, uintptr_t& moduleBase, HANDLE hProc)
{
    Player player;
    d3dContext.d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
    d3dContext.d3ddev->BeginScene();

    uintptr_t viewMatrixAddr = moduleBase + 0x101AE8;
    player.Matrix = Memory::RPM<ViewMatrix>(hProc, viewMatrixAddr);

    uintptr_t entitylist = moduleBase + 0x10f4f8;
	uintptr_t listEntry = Memory::RPM<uintptr_t>(hProc, entitylist);
    
    uintptr_t localplayer = moduleBase + 0x104f4;
    player.baseAdress = Memory::RPM<uintptr_t>(hProc, localplayer);
    player.Origin = Memory::RPM<Vec3>(hProc, player.baseAdress + 0x4);
    player.Angles.X = Memory::RPM<float>(hProc, player.baseAdress + 0x40);
    player.Angles.Y = Memory::RPM<float>(hProc, player.baseAdress + 0x44);

    std::vector<Entity> entity(32);

    for (size_t i = 0; i < 32; ++i)
    {
        entity[i].baseAdress = Memory::RPM<uintptr_t>(hProc, listEntry + (i * 0x4));
        entity[i].health = Memory::RPM<int>(hProc, entity[i].baseAdress + 0xf8);
        entity[i].maxHealth = 100;
        entity[i].Origin = Memory::RPM<Vec3>(hProc, entity[i].baseAdress + 0x34);

        if (entity[i].health <= 0)
            continue;

        Vec2 NDC;

        
        if (WorldToScreenEsp(entity[i].Origin, NDC, player.Matrix.Matrix, 2560, 1440))
        {
            DrawLine(d3dContext.d3ddev, 2560/2, 1440, NDC.X, NDC.Y);
            std::cout << NDC.X << std::endl;
            std::cout << NDC.Y << std::endl;
        }
    }

    d3dContext.d3ddev->EndScene();
    d3dContext.d3ddev->Present(NULL, NULL, NULL, NULL);
}



// Clean Up Direct3D
void cleanD3D(D3DContext& d3dContext)
{
    d3dContext.d3ddev->Release();
    d3dContext.d3d->Release();
}
