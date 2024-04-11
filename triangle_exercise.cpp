#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <assert.h>
using namespace std;
//#define CL_CALL(func) func;checkERRor();


/*
�����������Vertex Array Object��VAO
���㻺�����Vertex Buffer Object��VBO
Ԫ�ػ������Element Buffer Object��EBO �� ����������� Index Buffer Object��IBO
*/


//���û��ı䴰�ڵĴ�С��ʱ���ӿ�ҲӦ�ñ�����
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void checkError();
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//����ɫ������GLSL��д������ɫ����Դ����
//��ʱ��������ɫ����Դ����Ӳ������C����ַ����У�
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
	//    //��ʼ��GLFW������GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// ����һ�����ڶ�����GLFW����ʹ�á�������ڶ����������кʹ�����ص�����
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);//�����괰�����ǾͿ���֪ͨGLFW�����Ǵ��ڵ�����������Ϊ��ǰ�̵߳����������ˡ�
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  //����GLFW����ϣ��ÿ�����ڵ�����С��ʱ�����framebuffer_size_callback����
	// glad: load all OpenGL function pointers
  //GLAD����������OpenGL�ĺ���ָ��ģ������ڵ����κ�OpenGL�ĺ���֮ǰ������Ҫ��ʼ��GLAD��
	//ʹ��glad�������е�ǰ�汾opengl�ĺ���
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//����������ɫ����Ŀ  ������ʱ��̬��������Դ���롣
	//------------------------
	//������ɫ��

	//����һ����ɫ��������ID�����õġ��������Ǵ������������ɫ��Ϊunsigned int��Ȼ����glCreateShader���������ɫ����
	//���ǰ� ��Ҫ��������ɫ������GL_VERTEX_SHADER   �Բ�����ʽ�ṩ��glCreateShader�������������ڴ���һ��������ɫ�������ݵĲ�����GL_VERTEX_SHADER��
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//��һ�����ǰ������ɫ��Դ�븽�ӵ���ɫ�������ϣ�Ȼ���������
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	//���������
	int  success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	//��glGetShaderiv����Ƿ����ɹ����������ʧ�ܣ����ǻ���glGetShaderInfoLog��ȡ������Ϣ��Ȼ���ӡ����
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//����Ƭ����ɫ��
	unsigned int fragmentShader1, fragmentShader2;
	fragmentShader1 = glCreateShader(GL_FRAGMENT_SHADER);
	fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);

	//����Ƭ����ɫ������
	glShaderSource(fragmentShader1, 1, &fragmentShaderSource1, NULL);
	glShaderSource(fragmentShader2, 1, &fragmentShaderSource2, NULL);
	glCompileShader(fragmentShader1);
	glCompileShader(fragmentShader2);
	//�����ɫ���������
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
	//�����������
	unsigned int shaderProgram1; 
	unsigned int shaderProgram2;
	shaderProgram1 = glCreateProgram();
	shaderProgram2 = glCreateProgram();
	//glCreateProgram��������һ�����򣬲������´�����������ID���á�
	//����������Ҫ��֮ǰ�������ɫ�����ӵ���������ϣ�Ȼ����glLinkProgram�������ǣ�
	glAttachShader(shaderProgram1, vertexShader);
	glAttachShader(shaderProgram1, fragmentShader1);
	glAttachShader(shaderProgram2, fragmentShader2);
	//����
	glLinkProgram(shaderProgram1);
	glLinkProgram(shaderProgram2);
	//������Ӵ���
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
	//ɾ����ɫ������
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader1);
	glDeleteShader(fragmentShader2);
	// set up vertex data (and buffer(s)) and configure vertex attributes
   // ------------------------------------------------------------------


	//���������ο�ʼ---------------------------------------------------------------------------------------------------------------------------------------------------
	float vertices[] = {
		//��һ��������
		-0.5f, 0.5f, 0.0f, // left  
		 -0.75f, -0.5f, 0.0f, // right 
		 -0.25f, -0.5f, 0.0f, // top  
		//�ڶ���������
		 0.5f, 0.5f, 0.0f, // left  
		 0.75f, -0.5f, 0.0f, // right 
		 0.25f, -0.5f, 0.0f  // top  
	};

	//���㻺�����(Vertex Buffer Objects, VBO)��������ڴ�
	unsigned int VBOS[2], VAOS[2];
	glGenVertexArrays(2, VAOS);
	glGenBuffers(2, VBOS); 

	// ��VAO�������
	glBindVertexArray(VAOS[0]);
	//��VOB�������
	glBindBuffer(GL_ARRAY_BUFFER, VBOS[0]);
	//���VBO����								  
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//���ö�������ָ��
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	// ��VAO�������
	glBindVertexArray(VAOS[1]);
	//��VOB�������
	glBindBuffer(GL_ARRAY_BUFFER, VBOS[1]);
	//���VBO����								  
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//���ö�������ָ��
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(9*sizeof(float))); //��ƫ��������ڶ�����������
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
	//����ϣ�����������������ر���֮ǰ���ϻ���ͼ���ܹ������û����롣��ˣ�������Ҫ�ڳ��������һ��whileѭ�������ǿ��԰�����֮Ϊ��Ⱦѭ��(Render Loop)��
	//������������GLFW�˳�ǰһֱ�������С����漸�еĴ����ʵ����һ���򵥵���Ⱦѭ����
	while (!glfwWindowShouldClose(window))  //����������ÿ��ѭ���Ŀ�ʼǰ���һ��GLFW�Ƿ�Ҫ���˳�������ǵĻ����ú�������true����Ⱦѭ����ֹͣ���У�֮�����ǾͿ��Թر�Ӧ�ó���
	{
		// ��Ҫʵ��һЩ������ƣ�ͨ��ʹ��GLFW�ļ������뺯������ɡ����ǽ���ʹ��GLFW��glfwGetKey����������Ҫһ�������Լ�һ��������Ϊ���롣
		// ����������᷵����������Ƿ����ڱ����¡����ǽ�����һ��processInput�����������е�������뱣�����ࡣ
		// -----
		processInput(window);
		//��Ⱦָ����ÿ����Ⱦѭ��������ʱ���ܱ�ִ��
		//glClearColor��״̬���ú�������glClear��������һ��״̬ʹ�õĺ�������ʹ���˵�ǰ��״̬()����ȡӦ�����Ϊ����ɫ�����������ɫ����֮��������ɫ���嶼�ᱻ���ΪglClearColor�������õ���ɫ
		//���û����������ɫ
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);  //����glClear�����������Ļ����ɫ���壬������һ������λ(Buffer Bit)��ָ��Ҫ��յĻ���
		//ִ���������Ĳ���
		glClear(GL_COLOR_BUFFER_BIT); //�ܵĻ���λ��GL_COLOR_BUFFER_BIT��GL_DEPTH_BUFFER_BIT��GL_STENCIL_BUFFER_BIT��������������ֻ������ɫֵ����������ֻ�����ɫ���塣
		
		//checkError();
		//����һ��������	
		glUseProgram(shaderProgram1);
		glBindVertexArray(VAOS[0]);
		glDrawArrays(GL_POINTS, 0, 3);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//���ڶ���
		glUseProgram(shaderProgram2);
		glBindVertexArray(VAOS[1]);
		glDrawArrays(GL_POINTS, 0, 3);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		//glDrawArrays(GL_LINE_LOOP, 0, 6);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		//�л�˫���棬�����˹��̣�ֻ��ʾ��ת��Ľ��
		glfwSwapBuffers(window);//�����ύ����ɫ���壨����һ��������GLFW����ÿһ��������ɫֵ�Ĵ󻺳壩��������һ�����б��������ƣ����ҽ�����Ϊ�����ʾ����Ļ�ϡ�
		glfwPollEvents(); //���������û�д���ʲô�¼�������������롢����ƶ��ȣ������´���״̬�������ö�Ӧ�Ļص�����
	}
	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(2, VAOS);
	glDeleteBuffers(2, VBOS);
	glDeleteProgram(shaderProgram1);
	glDeleteProgram(shaderProgram2);


	// glfw: terminate, clearing all previously allocated GLFW resources.��Ⱦѭ��������������Ҫ��ȷ�ͷ�/ɾ��֮ǰ�ķ����������Դ
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}
//���������ν���	--------------------------------------------------------------------------------------------------------------------------------------------------


void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ���û��ı䴰�ڵĴ�С��ʱ���ӿ�ҲӦ�ñ�����
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}
//������
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
		//assert����ݴ����boolֵ�����������Ƿ�ֹͣ
		assert(false);
	}

}