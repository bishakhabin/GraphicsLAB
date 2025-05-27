#include <glad.h>
#include <glfw3.h>
#include <iostream>

// Window dimensions
const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

// Paddle variables
float paddleWidth = 10.0f, paddleHeight = 100.0f;
float leftPaddleY = 250.0f, rightPaddleY = 250.0f;
float paddleSpeed = 500.0f;

// Ball variables
float ballX = 400.0f, ballY = 300.0f;
float ballSize = 10.0f;
float ballVelX = 300.0f, ballVelY = 300.0f;

float deltaTime = 0.0f, lastFrame = 0.0f;

// Score and state
int leftScore = 0, rightScore = 0;
bool gameEnded = false;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    if (gameEnded) return;

    float move = paddleSpeed * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && (leftPaddleY + paddleHeight + move <= HEIGHT))
        leftPaddleY += move;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && (leftPaddleY - move >= 0))
        leftPaddleY -= move;

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && (rightPaddleY + paddleHeight + move <= HEIGHT))
        rightPaddleY += move;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && (rightPaddleY - move >= 0))
        rightPaddleY -= move;
}

void drawRect(float x, float y, float w, float h) {
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x, y + h);
    glEnd();
}

void updateBall() {
    if (gameEnded) return;

    ballX += ballVelX * deltaTime;
    ballY += ballVelY * deltaTime;

    if (ballY < 0 || ballY + ballSize > HEIGHT)
        ballVelY *= -1;

    if (ballX <= 20 && ballY + ballSize >= leftPaddleY && ballY <= leftPaddleY + paddleHeight)
        ballVelX *= -1;

    if (ballX + ballSize >= WIDTH - 20 && ballY + ballSize >= rightPaddleY && ballY <= rightPaddleY + paddleHeight)
        ballVelX *= -1;

    if (ballX < 0) {
        rightScore++;
        ballX = WIDTH / 2 - ballSize / 2;
        ballY = HEIGHT / 2 - ballSize / 2;
        ballVelX = 300.0f;
    }

    if (ballX > WIDTH) {
        leftScore++;
        ballX = WIDTH / 2 - ballSize / 2;
        ballY = HEIGHT / 2 - ballSize / 2;
        ballVelX = -300.0f;
    }

    if (leftScore >= 3 || rightScore >= 3)
        gameEnded = true;
}

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Colorful Pong", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    glViewport(0, 0, WIDTH, HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);
        updateBall();

        // Background color
        glClearColor(0.05f, 0.05f, 0.1f, 1.0f); // Dark blue background
        glClear(GL_COLOR_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, WIDTH, 0, HEIGHT, -1, 1);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        // Left paddle - Red
        glColor3f(1.0f, 0.2f, 0.2f);
        drawRect(10, leftPaddleY, paddleWidth, paddleHeight);

        // Right paddle - Green
        glColor3f(0.2f, 1.0f, 0.2f);
        drawRect(WIDTH - 20, rightPaddleY, paddleWidth, paddleHeight);

        // Ball - Yellow
        glColor3f(1.0f, 1.0f, 0.0f);
        drawRect(ballX, ballY, ballSize, ballSize);

        // Game over overlay
        if (gameEnded) {
            glColor3f(0.9f, 0.0f, 0.0f); // Red overlay
            drawRect(WIDTH / 2 - 150, HEIGHT / 2 - 30, 300, 60);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}