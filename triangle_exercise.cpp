#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <assert.h>
using namespace std;
//#define CL_CALL(func) func;checkERRor();


/*
顶点数组对象：Vertex Array Object，VAO
顶点缓冲对象：Vertex Buffer Object，VBO
元素缓冲对象：Element Buffer Object，EBO 或 索引缓冲对象 Index Buffer Object，IBO
*/


//当用户改变窗口的大小的时候，视口也应该被调整
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void checkError();
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//用着色器语言GLSL编写顶点着色器的源代码
//暂时将顶点着色器的源代码硬编码在C风格字符串中：
const char* vertexShaderSource = 
									"#version 330 core\n"
									"layout (location = 0) in vec3 aPos;\n"
									"void main()\n"
									"{\n"
									"gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);"
									"gl_PointSize = 2.0f;"
									"}\0";


const char* fragmentShaderSource1 = "#version 330 core\n"
									"out vec4 FragColor;\n"
									"void main()\n"
									"{\n"
									"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
									"}\n\0";

const char* fragmentShaderSource2 = "#version 330 core\n"
									"out vec4 FragColor;\n"
									"void main()\n"
									"{\n"
									"   FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
									"}\n\0";


int main()
{
	// glfw: initialize and configure
	//    //初始化GLFW和配置GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// 创建一个窗口对象，让GLFW函数使用。这个窗口对象存放了所有和窗口相关的数据
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);//创建完窗口我们就可以通知GLFW将我们窗口的上下文设置为当前线程的主上下文了。
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  //告诉GLFW我们希望每当窗口调整大小的时候调用framebuffer_size_callback函数
	// glad: load all OpenGL function pointers
  //GLAD是用来管理OpenGL的函数指针的，所以在调用任何OpenGL的函数之前我们需要初始化GLAD。
	//使用glad加载所有当前版本opengl的函数
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//建立编译着色器项目  在运行时动态编译它的源代码。
	//------------------------
	//顶点着色器

	//创建一个着色器对象，用ID来引用的。所以我们储存这个顶点着色器为unsigned int，然后用glCreateShader创建这个着色器：
	//我们把 需要创建的着色器类型GL_VERTEX_SHADER   以参数形式提供给glCreateShader。由于我们正在创建一个顶点着色器，传递的参数是GL_VERTEX_SHADER。
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//下一步我们把这个着色器源码附加到着色器对象上，然后编译它：
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	//检查编译错误
	int  success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	//用glGetShaderiv检查是否编译成功。如果编译失败，我们会用glGetShaderInfoLog获取错误消息，然后打印它。
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//创建片段着色器
	unsigned int fragmentShader1, fragmentShader2;
	fragmentShader1 = glCreateShader(GL_FRAGMENT_SHADER);
	fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);

	//附加片段着色器代码
	glShaderSource(fragmentShader1, 1, &fragmentShaderSource1, NULL);
	glShaderSource(fragmentShader2, 1, &fragmentShaderSource2, NULL);
	glCompileShader(fragmentShader1);
	glCompileShader(fragmentShader2);
	//检查着色器编译错误
	glGetShaderiv(fragmentShader1, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(fragmentShader1, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	glGetShaderiv(fragmentShader2, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(fragmentShader2, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	//创建程序对象
	unsigned int shaderProgram1; 
	unsigned int shaderProgram2;
	shaderProgram1 = glCreateProgram();
	shaderProgram2 = glCreateProgram();
	//glCreateProgram函数创建一个程序，并返回新创建程序对象的ID引用。
	//现在我们需要把之前编译的着色器附加到程序对象上，然后用glLinkProgram链接它们：
	glAttachShader(shaderProgram1, vertexShader);
	glAttachShader(shaderProgram1, fragmentShader1);
	glAttachShader(shaderProgram2, fragmentShader2);
	//链接
	glLinkProgram(shaderProgram1);
	glLinkProgram(shaderProgram2);
	//检查链接错误
	glGetProgramiv(shaderProgram1, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram1, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glGetProgramiv(shaderProgram2, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram2, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	//
	//删除着色器对象
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader1);
	glDeleteShader(fragmentShader2);
	// set up vertex data (and buffer(s)) and configure vertex attributes
   // ------------------------------------------------------------------


	//绘制三角形开始---------------------------------------------------------------------------------------------------------------------------------------------------
	float vertices[] = {
		//第一个三角形
		-0.5f, 0.5f, 0.0f, // left  
		 -0.75f, -0.5f, 0.0f, // right 
		 -0.25f, -0.5f, 0.0f, // top  
		//第二个三角形
		 0.5f, 0.5f, 0.0f, // left  
		 0.75f, -0.5f, 0.0f, // right 
		 0.25f, -0.5f, 0.0f  // top  
	};

	//顶点缓冲对象(Vertex Buffer Objects, VBO)管理这个内存
	unsigned int VBOS[2], VAOS[2];
	glGenVertexArrays(2, VAOS);
	glGenBuffers(2, VBOS); 

	// 绑定VAO缓冲对象
	glBindVertexArray(VAOS[0]);
	//绑定VOB缓冲对象
	glBindBuffer(GL_ARRAY_BUFFER, VBOS[0]);
	//填充VBO数据								  
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//设置顶点属性指针
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	// 绑定VAO缓冲对象
	glBindVertexArray(VAOS[1]);
	//绑定VOB缓冲对象
	glBindBuffer(GL_ARRAY_BUFFER, VBOS[1]);
	//填充VBO数据								  
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//设置顶点属性指针
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(9*sizeof(float))); //用偏移量代替第二个顶点数组
	glEnableVertexAttribArray(0);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);


	// uncomment this call to draw in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop
	// -----------
	//我们希望程序在我们主动关闭它之前不断绘制图像并能够接受用户输入。因此，我们需要在程序中添加一个while循环，我们可以把它称之为渲染循环(Render Loop)，
	//它能在我们让GLFW退出前一直保持运行。下面几行的代码就实现了一个简单的渲染循环：
	while (!glfwWindowShouldClose(window))  //函数在我们每次循环的开始前检查一次GLFW是否被要求退出，如果是的话，该函数返回true，渲染循环将停止运行，之后我们就可以关闭应用程序。
	{
		// 想要实现一些输入控制，通过使用GLFW的几个输入函数来完成。我们将会使用GLFW的glfwGetKey函数，它需要一个窗口以及一个按键作为输入。
		// 这个函数将会返回这个按键是否正在被按下。我们将创建一个processInput函数来让所有的输入代码保持整洁。
		// -----
		processInput(window);
		//渲染指令在每次渲染循环迭代的时候都能被执行
		//glClearColor是状态设置函数，而glClear函数则是一个状态使用的函数，它使用了当前的状态()来获取应该清除为的颜色。所以清除颜色缓冲之后，整个颜色缓冲都会被填充为glClearColor里所设置的颜色
		//设置画布清理的颜色
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);  //调用glClear函数来清空屏幕的颜色缓冲，它接受一个缓冲位(Buffer Bit)来指定要清空的缓冲
		//执行清理画布的操作
		glClear(GL_COLOR_BUFFER_BIT); //能的缓冲位有GL_COLOR_BUFFER_BIT，GL_DEPTH_BUFFER_BIT和GL_STENCIL_BUFFER_BIT。由于现在我们只关心颜色值，所以我们只清空颜色缓冲。
		
		//checkError();
		//画第一个三角形	
		glUseProgram(shaderProgram1);
		glBindVertexArray(VAOS[0]);
		glDrawArrays(GL_POINTS, 0, 3);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//画第二个
		glUseProgram(shaderProgram2);
		glBindVertexArray(VAOS[1]);
		glDrawArrays(GL_POINTS, 0, 3);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		//glDrawArrays(GL_LINE_LOOP, 0, 6);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		//切换双缓存，隐藏了过程，只显示跳转后的结果
		glfwSwapBuffers(window);//函数会交换颜色缓冲（它是一个储存着GLFW窗口每一个像素颜色值的大缓冲），它在这一迭代中被用来绘制，并且将会作为输出显示在屏幕上。
		glfwPollEvents(); //函数检查有没有触发什么事件（比如键盘输入、鼠标移动等）、更新窗口状态，并调用对应的回调函数
	}
	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(2, VAOS);
	glDeleteBuffers(2, VBOS);
	glDeleteProgram(shaderProgram1);
	glDeleteProgram(shaderProgram2);


	// glfw: terminate, clearing all previously allocated GLFW resources.渲染循环结束后我们需要正确释放/删除之前的分配的所有资源
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}
//绘制三角形结束	--------------------------------------------------------------------------------------------------------------------------------------------------


void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// 当用户改变窗口的大小的时候，视口也应该被调整
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}
//错误检查
void checkError()  
{
	GLenum errorCode = glGetError();
	std::string error = "";
	if (errorCode != GL_NO_ERROR) {
		switch (errorCode) {
		case GL_INVALID_ENUM: error = "INVALID_ENUM"; break;
		case GL_INVALID_VALUE: error = "INVALID VALUE"; break;
		case GL_INVALID_OPERATION: error = "INVALID OPERATION"; break;
		case GL_OUT_OF_MEMORY: error = "OUT OF MEMORY"; break;
		default:
			error = "UNKNOWN";
			break;
		}
		cout << error << endl;
		//assert会根据传入的bool值，决定程序是否停止
		assert(false);
	}

}