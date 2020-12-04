#include <glad.h>                           // GL header file
#include <GLFW/glfw3.h>                     // GL toolkit
#include <stdio.h>                          // printf, etc.
#include "GLXtras.h"                        // convenience routines
#include <iostream>
#include "input.h"
#include "stb_image.h"
#include "GameObject.h"
#include <vector>
#include "Button.h"
#include "GameGrid.h"
#include "input.h"
#include "Core.h"
#include "Gameplay.h"
#include "time.h"
#include "Card.h"


// Sound Engine to play audio
ISoundEngine* Audio = createIrrKlangDevice();


// Window Diminsions:
int WINDOW_WIDTH = 800;
int WINDOW_HEIGHT = 800;


Input input;
GameGrid* gameGrid;

unsigned int PLAYER1_PIECE_TEXTURE;
unsigned int PLAYER2_PIECE_TEXTURE;

// Render buffers to draw objects:
std::vector<GameObject*> render_buffer;
std::vector<GameObject*> render_buffer2;

// Textures addresses:
unsigned int* text_textures;
unsigned int* card_textures;
unsigned int* misc_textures;

// Callback functions:
static void mouse_pos_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);



void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void LoadTexture(unsigned int& textureID, std::string path, bool isFlipped, bool hasAlpha);
void ClearScreen();
void DrawSprite(GameObject* gameObject);
std::vector<GameObject*> InitializeMainMenu();
std::vector<GameObject*> InitializeGameScene();
void Renderer(std::vector<GameObject*> gameobjects);
void OnClickStartGame();
void CleanUp(std::vector<GameObject*>& gameObjects);
void InittializeTextTextures();
void InitializeCardTextures();
void InitializeMiscTextures();




GLuint gameBackgroundShader = 0;
GLuint spriteShader = 0;
GLuint gamegridShader = 0;






bool isStartGame = false;





float sprite_vertices[] =
{
  -1.0f,  1.0f,     // Top Left
   1.0f,  1.0f,     // Top Right
   1.0f, -1.0f,     // Bottom Right
  -1.0f, -1.0f      // Bottom Left
};

float sprite_uv[] =
{
    1.0f,  1.0f,    // Top Right
    1.0f,  0.0f,    // Bottom Right
    0.0f,  0.0f,    // Bottom Left
    0.0f,  1.0f     // Top Left
};

unsigned int sprite_indices[] =
{
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
};



int main()
{


    // Initialize OpenGL:
    glfwInit();
    
    // Set Window Settings:
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);


    // Create a window that has the settings we set:
    GLFWwindow* window = glfwCreateWindow(WINDOW_HEIGHT, WINDOW_WIDTH, "TicTacTwist", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Set the current Context:
    glfwMakeContextCurrent(window);

    // Initialize GLAD:
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Set the view port coordinates & dimensions:
    glViewport(0, 0, WINDOW_HEIGHT, WINDOW_WIDTH);



    // Set Callbacks:
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_pos_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);




    unsigned int VAO;
    unsigned int VBO_position;
    unsigned int VBO_uv;
    unsigned int EBO;

    // Create Buffers:
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO_position);
    glGenBuffers(1, &VBO_uv);
    glGenBuffers(1, &EBO);

    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);
   
    // VBO :
    glBindBuffer(GL_ARRAY_BUFFER, VBO_position);
    glBufferData(GL_ARRAY_BUFFER, sizeof(sprite_vertices), sprite_vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_uv);
    glBufferData(GL_ARRAY_BUFFER, sizeof(sprite_uv), sprite_uv, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    // EBO :
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sprite_indices), sprite_indices, GL_STATIC_DRAW);
 
    
    // For shader [position] [UV]:
    //glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    //glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);
    glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized








    InittializeTextTextures();
    InitializeCardTextures();
    InitializeMiscTextures();
    std::vector<GameObject*> mainmenu = InitializeMainMenu();
    std::vector<GameObject*> gamescene = InitializeGameScene();

    Gameplay gameplay(gameGrid);
    gameGrid->AssignGameplay(&gameplay);
    
    gameplay.AssignPlayer1TurnObject(gamescene[2]);
    gameplay.AssignPlayer2TurnObject(gamescene[3]);

    time_t seconds;
    seconds = time(NULL);

    ISoundSource* backgroundMusic;
    backgroundMusic = Audio->addSoundSourceFromFile("../../Resources/audio/background.mp3", ESM_AUTO_DETECT);
    backgroundMusic->setDefaultVolume(0.05f);
    Audio->play2D(backgroundMusic, true);



    // Initialize Input:
    input.initialize(window);
    //Test listene(&input.getMouse().onRightClick);

    // Main Loop:
    while (!glfwWindowShouldClose(window))
    {
        // Input:
        processInput(window);

        // Spinning Turn effect:
            
        gamescene[2]->SetRotation(0, 0, gamescene[2]->GetRotation().z + (10));
        gamescene[3]->SetRotation(0, 0, gamescene[2]->GetRotation().z + (10));



        // Rendering:
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        ClearScreen();
        glBindVertexArray(VAO);

        if (!isStartGame) Renderer(mainmenu);
        else
        {
            Renderer(gamescene);
            Renderer(render_buffer);
            Renderer(render_buffer2);
        }
      

        glFlush();
        // Buffer/Frame Updates:
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Garbage in, Garbage out:
    glfwTerminate();
    CleanUp(mainmenu);
    CleanUp(gamescene);
    
    delete[] text_textures;
    delete[] card_textures;
    delete[] misc_textures;
    return 0;
}



void ClearScreen()
{
    glClearColor(0.2f, 0.3f, 0.3f, 0.5f);
    glClear(GL_COLOR_BUFFER_BIT);
}



void DrawSprite(GameObject* gameObject)
{
    if (!gameObject->IsOn()) return;

    const char* vertexShader = R"(
        #version 330
		layout(location = 0) in vec2 point_pos;
		layout(location = 1) in vec2 point_uv;
        uniform mat4 mvp;
        out vec2 uv;
        void main() 
        {
            uv = point_uv;
            gl_Position = mvp * vec4(point_pos, 0, 1);
        }
    )";

    const char* fragmentShader = R"(
        #version 330
        in vec2 uv;
        out vec4 fragColor;
        uniform sampler2D image;
        void main() 
        {
            fragColor = texture(image, uv);
        }
    )";

    // Compile the shader program and link if haven't done already:
    if (!spriteShader) spriteShader = LinkProgramViaCode(&vertexShader, &fragmentShader);

    // Set Current Shader:
    glUseProgram(spriteShader);

    VertexAttribPointer(spriteShader, "point_pos", 2, 0, (void*)sizeof(sprite_vertices));
    VertexAttribPointer(spriteShader, "point_uv", 2, 0, (void*)sizeof(sprite_uv));

    // Set Transform:
    mat4 trans = Translate(gameObject->GetPosition().x, gameObject->GetPosition().y, gameObject->GetPosition().z);
    mat4 rot_x = RotateX(0 + gameObject->GetRotation().x);
    mat4 rot_y = RotateY(0 + gameObject->GetRotation().y);
    mat4 rot_z = RotateZ(-90 + gameObject->GetRotation().z);
    mat4 rot = (rot_x * rot_y) * rot_z;
    mat4 scaleMat = Scale(gameObject->GetScale());
    SetUniform(spriteShader, "mvp", trans * rot * scaleMat);
    
    // Texture:
    // To use a texture in the shader [OpenGL provides 16 channels]:
    // 1- Activate the texture channel to use
    // 2- set/bind the texture object to the channel
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gameObject->GetTexture());
    SetUniform(spriteShader, "image", 0);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}





void processInput(GLFWwindow* window)
{
    //std::cout << "xPos: " << input.getMouse().getXPosition() << " yPos: " << input.getMouse().getYPosition() << '\n';
}




void LoadTexture(unsigned int &textureID, std::string path, bool isFlipped, bool hasAlpha)
{
    // texture id and bind 
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_set_flip_vertically_on_load(isFlipped);


    int width, height, nrChannels;
    unsigned char* data;
    data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

    if (data) 
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        if(!hasAlpha) glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        else glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
}




// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, height, width);
}



void Renderer(std::vector<GameObject*> gameobjects)
{
    for (int i = 0; i < gameobjects.size(); i++)
    {
        DrawSprite(gameobjects[i]);
    }
}



void InittializeTextTextures()
{
    const std::string ZERO_TEXTURE = "../../Resources/GameInterface/Text/0.png";
    const std::string ONE_TEXTURE = "../../Resources/GameInterface/Text/1.png";
    const std::string TWO_TEXTURE = "../../Resources/GameInterface/Text/2.png";
    const std::string THREE_TEXTURE = "../../Resources/GameInterface/Text/3.png";
    const std::string FOUR_TEXTURE = "../../Resources/GameInterface/Text/4.png";
    const std::string FIVE_TEXTURE = "../../Resources/GameInterface/Text/5.png";
    const std::string X_TEXTURE = "../../Resources/GameInterface/Text/x.png";

    // Text Texture Loading 
    unsigned int TEXT_TEXTURE_SIZE = 7;
    text_textures = new unsigned int[TEXT_TEXTURE_SIZE];
    glGenTextures(TEXT_TEXTURE_SIZE, text_textures);
    LoadTexture(text_textures[0], ZERO_TEXTURE, true, true);
    LoadTexture(text_textures[1], ONE_TEXTURE, true, true);
    LoadTexture(text_textures[2], TWO_TEXTURE, true, true);
    LoadTexture(text_textures[3], THREE_TEXTURE, true, true);
    LoadTexture(text_textures[4], FOUR_TEXTURE, true, true);
    LoadTexture(text_textures[5], FIVE_TEXTURE, true, true);
    LoadTexture(text_textures[6], X_TEXTURE, true, true);
}


void InitializeCardTextures()
{
    const std::string EMPTY_TEXTURE = "../../Resources/Empty.png";
    const std::string REPOSITION_TEXTURE = "../../Resources/Card/Reposition.png";
    const std::string SWITCH_TEXTURE = "../../Resources/Card/Switch.png";
    const std::string LOCK_TEXTURE = "../../Resources/Card/Lock.png";

    // Text Texture Loading 
    unsigned int TEXTURE_SIZE = 4;
    card_textures = new unsigned int[TEXTURE_SIZE];
    glGenTextures(TEXTURE_SIZE, card_textures);
    LoadTexture(card_textures[0], EMPTY_TEXTURE, true, false);
    LoadTexture(card_textures[1], REPOSITION_TEXTURE, true, true);
    LoadTexture(card_textures[2], SWITCH_TEXTURE, true, true);
    LoadTexture(card_textures[3], LOCK_TEXTURE, true, true);
}



void InitializeMiscTextures()
{
    const std::string SELECTED_PIECE_TEXTURE = "../../Resources/GameInterface/SelectedPiece.png";
    const std::string LOCKED_PIECE_TEXTURE = "../../Resources/GameInterface/LockedPiece.png";
    const std::string CARD_SELECTED_TEXTURE = "../../Resources/GameInterface/CardSelected.png";
    const std::string DIM_TEXTURE = "../../Resources/GameInterface/Dim.png";
    const std::string WINNERBAR_TEXTURE = "../../Resources/GameInterface/WinnerBar.png";
    const std::string P1W_TEXTURE = "../../Resources/GameInterface/player1Win.png";
    const std::string P2W_TEXTURE = "../../Resources/GameInterface/player2Win.png";

    
    unsigned int TEXTURE_SIZE = 7;
    misc_textures = new unsigned int[TEXTURE_SIZE];
    glGenTextures(TEXTURE_SIZE, misc_textures);
    LoadTexture(misc_textures[0], SELECTED_PIECE_TEXTURE, true, true);
    LoadTexture(misc_textures[1], LOCKED_PIECE_TEXTURE, true, true);
    LoadTexture(misc_textures[2], CARD_SELECTED_TEXTURE, true, true);
    LoadTexture(misc_textures[3], DIM_TEXTURE, true, true);
    LoadTexture(misc_textures[4], WINNERBAR_TEXTURE, true, true);
    LoadTexture(misc_textures[5], P1W_TEXTURE, true, true);
    LoadTexture(misc_textures[6], P2W_TEXTURE, true, true);


}


std::vector<GameObject*>  InitializeMainMenu()
{
    std::vector<GameObject*> gameobjects;

    // Textures paths from computer:
    const std::string BACKGROUND_TEXTURE_PATH = "../../Resources/nightbackground.jpg";
    const std::string GAME_LOGO_PATH = "../../Resources/Logo.png";
    const std::string MM_STARTGAME_TEXTURE_PATH = "../../Resources/MainMenu/StartGame2.png";

    unsigned int* textures;
    // Loading textures from computer:
    int TEXTURE_SIZE = 3;

    textures = new unsigned int[TEXTURE_SIZE];
    glGenTextures(TEXTURE_SIZE, textures);
    LoadTexture(textures[0], BACKGROUND_TEXTURE_PATH, true, false);
    LoadTexture(textures[1], GAME_LOGO_PATH, true, true);
    LoadTexture(textures[2], MM_STARTGAME_TEXTURE_PATH, true, true);


    GameObject* background = new GameObject(textures[0]);
    GameObject* logo = new GameObject(textures[1]);
    Button* startGameButton = new Button(textures[2]);

    // Set Scale:
    logo->SetScale(0.65, 0.65, 0.65);
    startGameButton->SetScale(0.125, 0.125, 0.125);

    // Set Position:
    startGameButton->SetPosition(0, -0.7f, 0);

    

    gameobjects.push_back(background);
    gameobjects.push_back(logo);
    gameobjects.push_back(startGameButton);

    // Set Buttons callbacks:
    startGameButton->AddOnClickListener(OnClickStartGame);




    // Register Buttons:
    input.AddButton(startGameButton);

    delete[] textures;
    return gameobjects;
}


std::vector<GameObject*> InitializeGameScene()
{
    // Textures Paths on PC:
    const std::string BACKGROUND_TEXTURE_PATH = "../../Resources/nightbackground.jpg";
    const std::string GAMEGRID_TEXTURE_PATH = "../../Resources/GridBackground.png";
    const std::string PLAYER1_PATH = "../../Resources/GameInterface/Player1.png";
    const std::string PLAYER2_PATH = "../../Resources/GameInterface/Player2.png";
    const std::string TURN_SPINNER_PATH = "../../Resources/GameInterface/TurnSpinner.png";
    const std::string PLAYER1PFP_PATH = "../../Resources/GameInterface/Player1pfp.png";
    const std::string PLAYER2PFP_PATH = "../../Resources/GameInterface/Player2pfp.png";
    const std::string SQUARE_TEXTURE = "../../Resources/Square.png";
    const std::string PIECE1_TEXTURE = "../../Resources/GameInterface/Player1pfp.png";
    const std::string PIECE2_TEXTURE = "../../Resources/GameInterface/Player2pfp.png";



    std::vector<GameObject*> gameobjects;
    unsigned int* textures;

    // Texture Loading Process:
    int TEXTURE_SIZE = 8;
    textures = new unsigned int[TEXTURE_SIZE];
    glGenTextures(TEXTURE_SIZE, textures);
    LoadTexture(textures[0], BACKGROUND_TEXTURE_PATH, true, false);
    LoadTexture(textures[1], GAMEGRID_TEXTURE_PATH, true, false);
    LoadTexture(textures[2], PLAYER1_PATH, true, true);
    LoadTexture(textures[3], PLAYER2_PATH, true, true);
    LoadTexture(textures[4], PLAYER1PFP_PATH, true, true);
    LoadTexture(textures[5], PLAYER2PFP_PATH, true, true);
    LoadTexture(textures[6], SQUARE_TEXTURE, true, false);
    LoadTexture(PLAYER1_PIECE_TEXTURE, PIECE1_TEXTURE, true, true);
    LoadTexture(PLAYER2_PIECE_TEXTURE, PIECE2_TEXTURE, true, true);
    LoadTexture(textures[7], TURN_SPINNER_PATH, true, true);


    //std::cout << "ERROR: " << glGetError() << '\n';


    // Initialize GameObjects:
    GameObject* background = new GameObject(textures[0]);
    gameGrid = new GameGrid(textures[1],textures[6]);
    GameObject* turnSpinner1 = new GameObject(textures[7]);
    GameObject* turnSpinner2 = new GameObject(textures[7]);
    GameObject* player1_x = new GameObject(text_textures[6]);
    GameObject* player2_x = new GameObject(text_textures[6]);
    GameObject* player1pfp = new GameObject(textures[4]);
    GameObject* player2pfp = new GameObject(textures[5]);



    // Set Up GameObjects on Screen:
    turnSpinner2->SetRenderState(false);

    
    // Set Scale:
    gameGrid->SetScale(0.6, 0.6, 0.6);
    turnSpinner1->SetScale(0.13, 0.13, 0.1);
    turnSpinner2->SetScale(0.13, 0.13, 0.1);
    player1_x->SetScale(0.05, 0.05, 0.25);
    player2_x->SetScale(0.05, 0.05, 0.25);
    player1pfp->SetScale(0.09, 0.09, 0.065);
    player2pfp->SetScale(0.09, 0.09, 0.065);

    // Set Position:
    gameGrid->SetPosition(0, 0.1, 0);
    turnSpinner1->SetPosition(-0.8, 0.84, 0);
    turnSpinner2->SetPosition(0.6, 0.84, 0);
    player1_x->SetPosition(-0.62, 0.85, 0);
    player2_x->SetPosition(0.78, 0.85, 0);
    player1pfp->SetPosition(-0.8, 0.835, 0);
    player2pfp->SetPosition(0.6, 0.835, 0);

    // Add Gameobjects to the buffer:
    gameobjects.push_back(background);
    gameobjects.push_back(gameGrid);
    gameobjects.push_back(turnSpinner1);
    gameobjects.push_back(turnSpinner2);
    gameobjects.push_back(player1_x);
    gameobjects.push_back(player2_x);
    gameobjects.push_back(player1pfp);
    gameobjects.push_back(player2pfp);


    // This snippet was used to test Creating the grid Grid mathmatically:

    //for (int c = 0; c < 5; c++)
    //{
    //    for (int r = 0; r < 5; r++)
    //    {
    //        double x_scale = (gameGrid->GetScale().x / 5.0);
    //        double y_scale = (gameGrid->GetScale().y / 5.0);
    //        double x_pos = gameGrid->GetPosition().x + 2 * ( (-(gameGrid->GetScale().x / 2.0)) + c * x_scale) + x_scale;
    //        double y_pos = gameGrid->GetPosition().y + 2 * ( ((gameGrid->GetScale().y / 2.0)) - r * y_scale) - y_scale;
    //        GameObject* square = new GameObject(textures[6]);
    //        square->SetPosition(x_pos, y_pos, 0);
    //        square->SetScale(x_scale - 0.01 , y_scale - 0.01, 0);
    //        gameobjects.push_back(square);
    //    }
    //}


    gameGrid->Initialize();

    
    // Garbage in, Garbage out:
    delete[] textures;

    return gameobjects;
}



void OnClickStartGame()
{
    isStartGame = true;
}






static void mouse_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
    input.getMouse().updatePosition(xpos, ypos);
};

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    // Left mouse button:
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) input.OnMouseLeftClick();

    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) input.getMouse().onRightClick.notify();
}


void CleanUp(std::vector<GameObject*>& gameObjects)
{
    for (int i = 0; i < gameObjects.size(); i++) delete gameObjects[i];
}