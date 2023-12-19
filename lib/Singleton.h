//
// Created by watson on 12/3/23.
//

#include <utility>
#include <memory>

/**
 * Namespace specifically for the Singleton
 */
namespace s{
    template<class T> //Define Template

    /**
     * Single ton class
     * @tparam T , Used class template
     */
    class Singleton{
    private:
        /**
         * Pointers to enforce Singleton
         */
        static T* _this;
        static std::shared_ptr<T> _instance;

        /**
         * Private Constructor
         */
        Singleton(){};

    public:
        /**
         * Deleted constructors
         */
        Singleton(const Singleton&) = delete;
        Singleton& operator=(const Singleton&) = delete;

        template <typename... Args> /*!< Use args so we can use every constructor for every T */

        /**
         * Get the only Instance running
         * @tparam Args template for the parameter pack
         * @param args parameter pack
         * @return
         */
        static std::shared_ptr<T> get(const Args&&... args){
            if(_this == nullptr){
                _this = new T(std::forward<Args>(args)...);
                _instance = std::shared_ptr<T>(_this);
            }
            return _instance;
        };

        /**
         * Simple destructor
         */
        virtual ~Singleton() = default;
    };

    template<class T> T* Singleton<T>::_this = nullptr; /*!< Static members should be initialized outside the class */
    template<class T> std::shared_ptr<T> Singleton<T>::_instance = nullptr;
}


