#ifndef COMPUBRITE_SFML_COLLISIONSYSTEM_H
#define COMPUBRITE_SFML_COLLISIONSYSTEM_H

#include <CompuBrite/hash_util.h>
#include <CompuBrite/SFML/ISystem.h>

#include <SFML/Graphics/Rect.hpp>

#include <typeinfo.h>
#include <unordered_map>
#include <functional>

namespace CompuBrite
{
namespace SFML
{

class CollisionSystem : public CompuBrite::SFML::ISystem
{
public:
    enum Level {AABB, BB, SAT};
    using TypeIDs = std::pair<const std::type_info*, const std::type_info*>;
    using Handler = std::pair<bool, std::function<void(IEntity &, IEntity&, const sf::FloatRect &)>>;

    explicit CollisionSystem(Level level);
    virtual ~CollisionSystem() = default;

    void update(sf::Time dt) override;

    template<typename T1, typename T2>
    void addHandler(std::function<void(T1&, T2&, const sf::FloatRect &)> callback)
    {
        auto t1 = &typeid(T1);
        auto t2 = &typeid(T2);
        auto handler = Handler(t2 < t1,
                    [callback] (IEntity& e1, IEntity &e2, const sf::FloatRect &rect)
                    {
                        Dispatcher<T1, T2> cd(callback);
                        cd(e1, e2, rect);
                    });
        auto types = std::minmax(t1, t2);
        handlers_[types] = handler;
    }

private:
    template <typename T1, typename T2>
    class Dispatcher
    {
    public:
        using Handler = std::function<void(T1 &, T2&, const sf::FloatRect &)>;

        Dispatcher(const Handler &handler) :
            handler_(handler)
        { }

        void operator()(IEntity &e1, IEntity &e2, const sf::FloatRect &rect)
        {
            handler_(static_cast<T1&>(e1), static_cast<T2&>(e2), rect);
        }
    private:
        Handler handler_;
    };

private:
    using Handlers = std::unordered_map<TypeIDs, Handler>;

    bool didCollide(const IEntity &lhs, const IEntity &rhs, sf::FloatRect &rect) const;
    void checkCollisions() const;
    void handleCollision(IEntity &lhs, IEntity &rhs, const sf::FloatRect &rect) const;
    Handler find(const TypeIDs &p) const;

    Handlers handlers_;
    Level level_;
};

} // namespace SFML
} // namespace CompuBrite

#endif // COMPUBRITE_SFML_COLLISIONSYSTEM_H
