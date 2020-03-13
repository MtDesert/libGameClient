#ifndef SHADER_H
#define SHADER_H

#include"DataBlock.h"
#include"Array.h"
//注:在包含gl.h前,此宏必须定义,不然shader相关的内容则无法使用
#define GL_GLEXT_PROTOTYPES 1
#include"gl.h"

class ShaderProgram;
//着色器,调用硬件提供的功能
class Shader{
	GLuint shader;//着色器的ID
	friend class ShaderProgram;
public:
	Shader();

	//载入shader源码(类型,源码,错误回调函数)
	bool loadShaderString(GLenum shaderType,const DataBlock &sourceCode,WhenErrorString whenError=nullptr);
	//载入shader源码(类型,文件名,错误回调函数)
	bool loadShaderFile(GLenum shaderType,const GLchar *filename,WhenErrorString whenError=nullptr);
	void deleteShader();//删除shader
};

//着色器程序
class ShaderProgram{
	GLuint program;
public:
	ShaderProgram();
	//载入程序
	bool loadProgram(const Array<Shader> &shaderArr,WhenErrorString whenError=nullptr);
	bool useProgram(WhenErrorString whenError=nullptr)const;
	void deleteProgram();//删除shader程序
};
#endif//SHADER_H