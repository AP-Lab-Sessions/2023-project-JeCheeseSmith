//
// Created by watson on 11/22/23.
//

#ifndef PACMAN_OBSERVER_H
#define PACMAN_OBSERVER_H

#include <memory>
#include <vector>

/**
 * Forward Declarations
 */
namespace em{
    class EntityModel;
    class Ghost;
}

/**
 * Forward Declarations
 */
namespace w{
    typedef std::pair<int,int> Index;
    typedef std::pair<std::unique_ptr<em::Ghost>,Index> Sprite;
}

/**
 * Namespace used for the Observer Logic
 */
namespace obs {
    /**
     * Abstract Base Class for all Observers
     */
    class Observer {
    public:
        /**
         * Function to fetch the data of an observer
         * @param et
         */
        virtual void update(const em::EntityModel &et) const = 0;

        /**
         * Simple Destructor
         */
        virtual ~Observer() = default;
    };
}

#endif //PACMAN_OBSERVER_H
