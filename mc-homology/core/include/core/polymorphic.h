/// \file polymorphic.h
/// \brief A polymorphic type wrapper with value sematincs
///
/// This file contaings a simplified implementation of std::polymorphic
/// from c++26
#pragma once

#include <concepts>
#include <memory>
#include <utility>

namespace core {

#ifdef __cpp_lib_polymorphic

template<class T>
using Polymorphic = std::polymorphic<T>
#else

/// \brief A polymorphic class wrapper with value semantics
template<class T>
class Polymorphic {
public:
    /// \brief Constructs a default initialized object
    constexpr explicit Polymorphic()
        requires std::default_initializable<T>
        : Polymorphic(T {}) {}

    /// \brief Constructs the object from the object u
    template<class U = T>
        requires std::derived_from<U, T>
    constexpr explicit Polymorphic(U&& u) :
        m_object(std::make_unique<Model<U>>(std::forward<U>(u))) {}

    /// \brief Constructs the object in place
    template<class U, class... Args>
        requires std::derived_from<U, T>
    constexpr explicit Polymorphic(std::in_place_type_t<U>, Args&&... args) :
        m_object(std::make_unique<Model<U>>(std::forward<Args>(args)...)) {}

    /// \brief Copy constructor
    constexpr Polymorphic(Polymorphic const& other) :
        m_object(other.m_object->clone()) {}

    /// \brief Move constructor
    constexpr Polymorphic(Polymorphic&& other) noexcept :
        m_object(std::move(other.m_object)) {}

    /// \brief Copy assignment
    constexpr Polymorphic& operator=(Polymorphic const& other) {
        m_object = other.m_object->clone();
        return *this;
    }

    /// \brief Move assignment
    constexpr Polymorphic& operator=(Polymorphic&& other) noexcept {
        m_object = std::move(other.m_object);
        return *this;
    }

    /// \brief Arrow operator
    constexpr T const* operator->() const noexcept {
        return m_object->get_address();
    }

    /// \brief Arrow operator
    constexpr T* operator->() noexcept {
        return m_object->get_address();
    }

    /// \brief Dereference operator
    constexpr T const& operator*() const noexcept {
        return *m_object->get_address();
    }

    /// \brief Dereference operator
    constexpr T& operator*() noexcept {
        return *m_object->get_address();
    }

    /// \brief Returns true, if the object has been moved out of
    constexpr bool valueless_after_move() const noexcept {
        return m_object == nullptr;
    }

    /// \brief Swaps two objects together
    constexpr void swap(Polymorphic& other) noexcept {
        using std::swap;
        swap(m_object, other.m_object);
    }

private:
    struct Concept {
        constexpr virtual T const* get_address() const noexcept = 0;
        constexpr virtual T* get_address() noexcept = 0;
        constexpr virtual std::unique_ptr<Concept> clone() const = 0;
        constexpr virtual ~Concept() = default;
    };

    template<class Object>
    struct Model: public Concept {
        template<class U = Object>
        constexpr Model(U&& value) : m_value(std::forward<U>(value)) {}

        constexpr T const* get_address() const noexcept override {
            return std::addressof(m_value);
        }

        constexpr T* get_address() noexcept override {
            return std::addressof(m_value);
        }

        constexpr std::unique_ptr<Concept> clone() const override {
            return std::make_unique<Model<Object>>(m_value);
        }

    private:
        Object m_value;
    };

    std::unique_ptr<Concept> m_object;
};

#endif

} // namespace core
