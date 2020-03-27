#ifdef __MINGW32__
#else
#include"Shader.h"
#include"DataBlock.h"
#include"define.h"
#include"glu.h"

#define SHADER_ASSERT(code,errStr) \
if(!(code)){\
	WHEN_ERROR(string(errStr) + (char*)gluErrorString(glGetError()))\
	return false;\
}

static DataBlock shaderBlock;

//构造函数
Shader::Shader():shader(0){}
ShaderProgram::ShaderProgram():program(0){
	program=glCreateProgram();
}

bool Shader::loadShaderString(GLenum shaderType,const DataBlock &sourceCode,WhenErrorString whenError){
	printf("type: %u\n",shaderType);
	if(!glIsShader(shader)){//动态创建着色器
		shader=glCreateShader(shaderType);
		SHADER_ASSERT(shader,"glCreateShader: ")
	}
	//设定着色器源码
	auto codeStr=(const GLchar*)sourceCode.dataPointer;
	glShaderSource(shader,1,&codeStr,nullptr);
	SHADER_ASSERT(glGetError()==GL_NO_ERROR,"glShaderSource: ")
	//编译源码
	sourceCode.debug();
	glCompileShader(shader);
	GLint ok;
	glGetShaderiv(shader,GL_COMPILE_STATUS,&ok);
	if(ok!=GL_TRUE){//编译失败,获取错误信息
		if(whenError){//有错误回调时,才汇报错误信息
			GLint logLength=0;
			glGetShaderiv(shader,GL_INFO_LOG_LENGTH,&logLength);
			char buf[logLength+1];
			glGetShaderInfoLog(shader,sizeof(buf),nullptr,buf);
			whenError(string("glCompileShader: ")+buf);
		}
		return false;
	}
	return true;
}
bool Shader::loadShaderFile(GLenum shaderType, const GLchar *filename, WhenErrorString whenError){
	auto block=shaderBlock.loadFile(filename,whenError);
	if(block.dataPointer){
		bool ok=loadShaderString(shaderType,block,whenError);
		block.memoryFree();
		return ok;
	}
	return false;
}
void Shader::deleteShader(){
	glDeleteShader(shader);
	shader=0;
}

//着色器程序
bool ShaderProgram::loadProgram(const Array<Shader> &shaderArr,WhenErrorString whenError){
	if(!glIsProgram(program)){//创建着色器程序
		program=glCreateProgram();
		SHADER_ASSERT(program,"glCreateProgram: ")
	}
	//关联着色器
	for(auto &shader:shaderArr){
		glAttachShader(program,shader.shader);
		SHADER_ASSERT(glGetError()==GL_NO_ERROR,"glAttachShader: ")
	}
	//链接着色器
	glLinkProgram(program);
	GLint ok;
	glGetProgramiv(program,GL_LINK_STATUS,&ok);
	if(ok!=GL_TRUE){
		if(whenError){//有错误回调时,才汇报错误信息
			GLint logLength=0;
			glGetProgramiv(program,GL_INFO_LOG_LENGTH,&logLength);
			char buf[logLength+1];
			glGetProgramInfoLog(program,sizeof(buf),nullptr,buf);
			whenError(string("glLinkProgram: ")+buf);
		}
		return false;
	}
	return true;
}
bool ShaderProgram::useProgram(WhenErrorString whenError)const{
	glUseProgram(program);
	SHADER_ASSERT(glGetError()==GL_NO_ERROR,"glUseProgram: ")
	return true;
}
void ShaderProgram::deleteProgram(){
	GLsizei count=16;
	GLuint shaders[count];
	glGetAttachedShaders(program,count,&count,shaders);
	for(GLsizei i=0;i<count;++i){
		glDetachShader(program,shaders[i]);
	}
	glDeleteProgram(program);
	program=0;
}
#endif