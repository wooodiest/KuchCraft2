///
/// @file Shader.cpp
/// 
/// @author Michal Kuchnicki
/// 

#include "kcpch.h"
#include "Graphics/Data/Shader.h"
#include "Graphics/Data/ShaderLibrary.h"
#include "Graphics/Renderer.h"

#include <glad/glad.h>

namespace KuchCraft {

    /// Converts ShaderType enum to a human-readable string
    static std::string ShaderTypeToString(ShaderType type) 
    {
        switch (type)
        {
            case ShaderType::VERTEX:   return "VERTEX";
            case ShaderType::FRAGMENT: return "FRAGMENT";
            default:                   return "NONE";
        }
    }

    /// Converts ShaderType enum to OpenGL-specific GLenum
    static GLenum ShaderTypeToOpenGl(ShaderType type)
    {
        switch (type)
        {
            case ShaderType::VERTEX:   return GL_VERTEX_SHADER;
            case ShaderType::FRAGMENT: return GL_FRAGMENT_SHADER;
            default:                   return 0;
        }
    }

    Shader::Shader(const std::filesystem::path& filepath)
        : m_Filepath(filepath)
    {
        /// Extract the name of the shader from its file path.
        m_Name = m_Filepath.filename().replace_extension("").string();

        Compile();
    }

    Shader::~Shader()
    {
        glDeleteProgram(m_RendererID);
    }

    void Shader::Bind() const
    {
        glUseProgram(m_RendererID);
    }

    void Shader::Unbind() const
    {
        glUseProgram(0);
    }

    void Shader::ReCompile()
    {
        if (m_RendererID)
        {
            glDeleteProgram(m_RendererID);
            m_RendererID = 0;
        }

        Compile();
    }

    void Shader::SetInt(const std::string& name, int value)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform1i(location, value);
    }

    void Shader::SetIntArray(const std::string& name, int* values, uint32_t count)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform1iv(location, count, values);
    }

    void Shader::SetFloat(const std::string& name, float value)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform1f(location, value);
    }

    void Shader::SetFloat2(const std::string& name, const glm::vec2& value)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform2f(location, value.x, value.y);
    }

    void Shader::SetFloat3(const std::string& name, const glm::vec3& value)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform3f(location, value.x, value.y, value.z);
    }

    void Shader::SetFloat4(const std::string& name, const glm::vec4& value)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform4f(location, value.x, value.y, value.z, value.w);
    }

    void Shader::SetMat3(const std::string& name, const glm::mat3& value)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
    }

    void Shader::SetMat4(const std::string& name, const glm::mat4& value)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
    }

    void Shader::Compile()
    {
        /// Read the shader source from the specified file
        std::string source = ReadFile(m_Filepath);

        /// Preprocess the source code to handle includes and other directives
        Preprocess(source);

        ApplySubstitutions(source);

        /// Group the shader source code by type (vertex and fragment)
        auto sources = GroupByType(source);

        /// Buffer size for error messages
        constexpr int error_buffer_size = 512;

        /// Container with size for Vertex and Fragment shader IDs 
        std::vector<GLuint> shaderIDs;
        shaderIDs.reserve(2);

        /// Iterate over the grouped shader sources and compile them
        for (const auto& s : sources)
        {
            auto& [type, source] = s;

            if (source.empty() || type == ShaderType::NONE)
                continue;

            /// Compile the shader
            const char* shader_cstr = source.c_str();
            int shader = glCreateShader(ShaderTypeToOpenGl(type));
            glShaderSource(shader, 1, &shader_cstr, NULL);
            glCompileShader(shader);

            /// Check for compilation errors
            int success;
            char infoLog[error_buffer_size];
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, error_buffer_size, NULL, infoLog);
                Log::Error("{0} shader compilation failed : '{1}' : {2}", ShaderTypeToString(type), m_Name, infoLog);
                continue;
            }
            
            /// Add the successfully compiled shader to the list
            shaderIDs.push_back(shader);
        }

        //// Create the shader program and link the compiled shaders
        m_RendererID = glCreateProgram();

        /// Attach each shader to the program and link them together
        for (auto shaderID : shaderIDs)
            glAttachShader(m_RendererID, shaderID);

        glLinkProgram(m_RendererID);

        /// Check for linking errors
        int success;
        char infoLog[error_buffer_size];
        glGetProgramiv(m_RendererID, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(m_RendererID, error_buffer_size, NULL, infoLog);
            Log::Error("Shader linking failed : {0} : '{1}'", infoLog, m_Name);
            return;
        }
        
        /// Clean up the shaders after linking
        for (auto shaderID : shaderIDs)
            glDeleteShader(shaderID);
    }

    std::string Shader::ReadFile(const std::filesystem::path& filepath)
    {
        /// Variable to hold the shader source code
        std::string source;

        /// Open the file in binary mode
        std::ifstream in(filepath, std::ios::in | std::ios::binary);

        /// Check if the file was successfully opened
        if (in)
        {
            /// Move to the end of the file and get the size of the file
            in.seekg(0, std::ios::end);
            size_t size = in.tellg();

            /// Check if size retrieval was successful
            if (size != -1)
            {
                /// Resize the string to hold the file content, move back to the begining and read entire file into string
                source.resize(size);
                in.seekg(0, std::ios::beg);
                in.read(&source[0], size);
            }
            else
            {
                Log::Error("Could not read from file '{0}'", filepath.string());
                return std::string();
            }
        }
        else
        {
            Log::Error("Could not open file '{0}'", filepath.string());
            return std::string();
        }

        /// Return the complete shader source code
        return source;
    }

    void Shader::Preprocess(std::string& source)
    {
        /// Create variable to hold the processed shader source code and reserve memory for performance
        std::string processedSource;
        processedSource.reserve(source.size());

        /// Create a stream from the source code
        std::istringstream stream(source);

        /// Variable to hold each line of the source code
        std::string line;

        /// Read the source code line by line
        while (std::getline(stream, line))
        {
            /// Check for #include directive
            if (line.find("#include") != std::string::npos)
            {
                /// Find the starting and ending quote
                size_t start = line.find_first_of("\"");
                size_t end   = line.find_last_of("\"");

                /// Validate quote positions
                if (start != std::string::npos && end != std::string::npos && start < end)
                {
                    /// Extract the include file name and open it
                    std::string includeFile = line.substr(start + 1, end - start - 1);
                    std::ifstream includeStream(includeFile);

                    /// Check if the include file was successfully opened
                    if (includeStream.is_open())
                    {
                        /// Read the entire file into the buffer and append the content to the processed source
                        std::stringstream buffer;
                        buffer << includeStream.rdbuf();
                        processedSource += buffer.str();
                    }
                    else
                        Log::Error("Could not open include file: '{0}'", includeFile);
                }
            }
            else
                // Append the line to the processed source
                processedSource += line + "\n";
        }

        /// Update the original source with the processed content
        source = processedSource;
    }

    std::map<ShaderType, std::string> Shader::GroupByType(const std::string& source)
    {
        /// Tokens indicating shader types
        const std::string vertexToken   = "### VERTEX";
        const std::string fragmentToken = "### FRAGMENT";

        /// Map to hold shader type and their source code
        std::map<ShaderType, std::string> data;

        /// Find token positions
        size_t vertexPos   = source.find(vertexToken);
        size_t fragmentPos = source.find(fragmentToken);

        /// Check for tokens
        if (vertexPos == std::string::npos)
            Log::Error("No VERTEX tag in shader: '{0}'", m_Name);

        if (fragmentPos == std::string::npos)
            Log::Error("No FRAGMENT tag in shader: '{0}'", m_Name);

        /// Extract shader type source code
        data[ShaderType::VERTEX]   = source.substr(vertexPos + vertexToken.length(), fragmentPos - (vertexPos + vertexToken.length()));
        data[ShaderType::FRAGMENT] = source.substr(fragmentPos + fragmentToken.length());

        /// Return the map of shader types and their source code
        return data;
    }

    void Shader::ApplySubstitutions(std::string& source)
    {
        const auto& substitutions = Renderer::GetShaderSubstitutionMap();

        std::for_each(substitutions.begin(), substitutions.end(), [&](const auto& pair) {
            const auto& from = pair.first;
            const auto& to = pair.second;

            size_t pos = 0;
            while ((pos = source.find(from, pos)) != std::string::npos)
            {
                source.replace(pos, from.length(), to);
                pos += to.length();
            }
            });
    }

}
