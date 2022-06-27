/**
 * @file singleton.cpp
 * AUTHORS: Niv Kotek - 208236315
 * 
 */


#include <iostream>
using namespace std;
//https://codereview.stackexchange.com/questions/173929/modern-c-singleton-template


template < typename T >
class Singleton {
  public:
    static T& instance();
	static void Destroy();

    Singleton(const Singleton&) = delete;
    Singleton& operator= (const Singleton) = delete;
	Singleton(Singleton &&) = delete;
    Singleton & operator=(Singleton &&) = delete;

  protected:
    Singleton() { };

  private:
    inline static T * m_instance = nullptr;

    class MemGuard {
      public: 
        ~MemGuard() {
			if(m_instance)
			{
				std::cout << "Singleton::MemGuard::~MemGuard::destructed" << std::endl;
				delete m_instance;
          		m_instance = nullptr;

			}
        }
    };
};

#include <memory>
template<typename T>
T& Singleton<T>::instance()
{
	static MemGuard g; 
    if (!m_instance) {
        m_instance = new T(); 
      }
      return *m_instance;
}

template<typename T>
 void Singleton<T>::Destroy()
 {
	 if(m_instance)
	 {
		std::cout << "Singleton::Destroy::destructed" << std::endl;
		 delete m_instance;
        m_instance = nullptr;
	 }
 }

 #include <iostream>
class Test final : public Singleton<Test>
{
public:
    Test() { std::cout << "Test::constructed" << std::endl; }
    ~Test() {  std::cout << "Test::destructed" << std::endl; }

    void use() const { std::cout << "Test::in use" << std::endl; };
};


// ________________________test singleton____________________________________
int main()
{

    std::cout << "Entering main()" << std::endl;
    {
        auto const& t = Test::instance();
        t.use();
    }
    {
        auto const& t = Test::instance();
        t.use();
    }
    std::cout << "Leaving main()" << std::endl;
}
