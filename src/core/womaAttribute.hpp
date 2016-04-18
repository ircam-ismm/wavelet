/*
 * attribute.h
 *
 * Generic Attributes
 *
 * Contact:
 * - Jules Françoise <jules.francoise@ircam.fr>
 *
 * This code has been authored by <a href="http://julesfrancoise.com">Jules
 * Françoise</a>
 * in the framework of the <a href="http://skatvg.iuav.it/">SkAT-VG</a> European
 * project,
 * with <a href="frederic-bevilacqua.net">Frederic Bevilacqua</a>, in the
 * <a href="http://ismm.ircam.fr">Sound Music Movement Interaction</a> team of
 * the
 * <a href="http://www.ircam.fr/stms.html?&L=1">STMS Lab</a> - IRCAM - CNRS -
 * UPMC (2011-2015).
 *
 * Copyright (C) 2015 Ircam-Centre Pompidou.
 *
 * This File is part of Wavelet.
 *
 * Wavelet is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Wavelet is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Wavelet.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __xmm_lib__attribute__
#define __xmm_lib__attribute__

#include <boost/any.hpp>
#include <exception>
#include <limits>
#include <stdexcept>
#include <vector>

namespace woma {

#pragma mark -
#pragma mark === Functions checkLimits ===
///@cond DEVDOC

/**
 * @brief checks the validity of the requested value with respect to the current
 * limits
 * @param value requested attribute value
 * @param limit_min minimum value
 * @param limit_max maximum value
 */
template <typename T>
void checkLimits(T const& value, T const& limit_min, T const& limit_max) {
    throw std::runtime_error(
        "Attribute limits are not implemented for the current type.");
}

template <>
void checkLimits<std::size_t>(std::size_t const& value,
                              std::size_t const& limit_min,
                              std::size_t const& limit_max);

template <>
void checkLimits<bool>(bool const& value, bool const& limit_min,
                       bool const& limit_max);

template <>
void checkLimits<unsigned char>(unsigned char const& value,
                                unsigned char const& limit_min,
                                unsigned char const& limit_max);

template <>
void checkLimits<char>(char const& value, char const& limit_min,
                       char const& limit_max);

template <>
void checkLimits<unsigned int>(unsigned int const& value,
                               unsigned int const& limit_min,
                               unsigned int const& limit_max);

template <>
void checkLimits<int>(int const& value, int const& limit_min,
                      int const& limit_max);

template <>
void checkLimits<long>(long const& value, long const& limit_min,
                       long const& limit_max);

template <>
void checkLimits<float>(float const& value, float const& limit_min,
                        float const& limit_max);

template <>
void checkLimits<double>(double const& value, double const& limit_min,
                         double const& limit_max);

#pragma mark -
#pragma mark === Class AttributeBase ===
/**
 * @brief Base for Generic Attributes
 */
class AttributeBase {
  public:
    /**
     * @brief Constructor
     */
    AttributeBase() : changed(false) {}

    /**
     * @brief Defines if the value has been changed
     */
    bool changed;
};

#pragma mark -
#pragma mark === Class AttributeHandler ===
/**
 * @brief Attribute notification handler
 */
class AttributeHandler {
    template <typename T>
    friend class Attribute;

  public:
    /**
     * @brief Destructor
     */
    virtual ~AttributeHandler() {}

  protected:
    /**
     * @brief notification function called when a member attribute is changed
     */
    virtual void onAttributeChange(AttributeBase* attr_pointer) = 0;
};
///@endcond

#pragma mark -
#pragma mark === Class Attribute ===
/**
 * @brief Generic Attribute
 * @tparam T attribute type
 */
template <typename T>
class Attribute : public AttributeBase {
  public:
    /**
     * @brief Default Constructor
     * @param parent parent attribute handler (notified at attribute changes)
     * @param value attribute value
     * @param limit_min minimum limit
     * @param limit_max maximum limit
     */
    Attribute(AttributeHandler* parent = nullptr, T const& value = T(),
              T const& limit_min = default_limit_min(),
              T const& limit_max = default_limit_max())
        : limit_min_(limit_min), limit_max_(limit_max), parent_(parent) {
        set(value, true);
        changed = false;
    }

    /**
     * @brief Copy Constructor
     * @param src source attribute
     * @warning Also copies the attribute's parent object
     */
    Attribute(Attribute const& src)
        : value_(src.value_),
          limit_min_(src.limit_min_),
          limit_max_(src.limit_max_),
          parent_(src.parent_) {
        changed = false;
    }

    /**
     * @brief Assignment operator
     * @param src source attribute
     * @return copy of the source Attribute object
     * @warning Also copies the attribute's parent object
     */
    template <typename U>
    Attribute& operator=(Attribute<U> const& src) {
        if (this != &src) {
            value_ = src.value_;
            limit_min_ = src.limit_min_;
            limit_max_ = src.limit_max_;
            parent_ = src.parent_;
            changed = false;
        }
        return *this;
    }

    /**
     * @brief Destructor
     */
    virtual ~Attribute() {}

    /**
     * @brief Set the attribute value
     * @param value requested value
     * @param silently if true, don't notify the parent object
     * @throws domain_error exception if the value exceeds the limits
     * @throws runtime_error exception if the limit checking are not implemented
     * for the current type
     */
    void set(T const& value, bool silently = false) {
        checkLimits(value, limit_min_, limit_max_);
        value_ = value;
        changed = true;
        if (!silently && parent_) parent_->onAttributeChange(this);
    }

    /**
     * @brief get the attribute's current value
     * @return the attribute's current value
     */
    T get() const { return value_; }

    /**
     * @brief get a copy of the attribute's current value
     * @return a copy of the attribute's current value
     */
    T getCopy() const { return T(value_); }

    /**
     * @brief set the attribute's minimum value
     * @param limit_min minimum value
     */
    void set_limit_min(T const& limit_min = default_limit_min()) {
        limit_min_ = limit_min;
    }

    /**
     * @brief set the attribute's maximum value
     * @param limit_max maximum value
     */
    void set_limit_max(T const& limit_max = default_limit_max()) {
        limit_max_ = limit_max;
    }

    /**
     * @brief set the attribute's limit values
     * @param limit_min minimum value
     * @param limit_max maximum value
     */
    void set_limits(T const& limit_min = default_limit_min(),
                    T const& limit_max = default_limit_max()) {
        set_limit_min(limit_min);
        set_limit_max(limit_max);
    }

    /**
     * @brief set the parent to receive change notifications
     * @param parent parent object
     */
    void set_parent(AttributeHandler* parent) { parent_ = parent; }

    /**
     * @brief get the parent to receive change notifications
     * @return parent object
     */
    AttributeHandler* get_parent() const { return parent_; }

  protected:
    ///@cond DEVDOC

    /**
     * @brief Attribute default minimum value
     */
    static T default_limit_min() { return std::numeric_limits<T>::lowest(); }

    /**
     * @brief Attribute default maximum value
     */
    static T default_limit_max() { return std::numeric_limits<T>::max(); }

    /**
     * @brief Minimum value of the attribute
     */
    T limit_min_;

    /**
     * @brief Maximum value of the attribute
     */
    T limit_max_;

    /**
     * @brief Current value of the attribute
     */
    T value_;

    /**
     * @brief Listeners to be notified at attribute changes
     */
    AttributeHandler* parent_;

    ///@endcond
};
}

#endif
