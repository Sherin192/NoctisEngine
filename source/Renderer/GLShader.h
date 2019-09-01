
namespace noctis::rdr
{
	//Example empty GL interface of Base class for NoctisShaders
	//TODO: GLIMPL implement when adding openGL support
	/*
	template <typename ShaderType>
	class GLShader
	{
	public:
		GLShader(std::shared_ptr<RenderDevice>& renderDevice, std::wstring& source)
			:m_file(source)
		{
			Init(renderDevice);
		}
		GLShader(std::wstring&& source)
			:m_file(std::move(source))
		{
			Init();
		}

		void Bind(std::shared_ptr<RenderDevice>&) {  };
	private:
		void Init()
		{
			ShaderType* shaderType = static_cast<ShaderType*>(this);
			std::cout << "GL implementation here" << std::endl;
		};
	private:
		const std::filesystem::path								m_file;
		//GL data here
	};
	*/
}