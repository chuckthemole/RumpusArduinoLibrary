#pragma once
#include <Arduino.h>
#include <vector>
#include "InputType.h"

/**
 * @brief Represents a logical group of InputType instances.
 *
 * InputGroup allows grouping multiple inputs together, e.g., a directional pad,
 * a set of buttons, or multiple sensors that collectively trigger a higher-level action.
 */
class InputGroup
{
public:
    InputGroup() = default;
    ~InputGroup() = default;

    /**
     * @brief Add an InputType to this group.
     * @param input Pointer to an InputType instance. Must remain valid for the lifetime of the group.
     */
    void addInput(InputType *input)
    {
        if (input)
        {
            _inputs.push_back(input);
        }
    }

    /**
     * @brief Remove an InputType from this group.
     * @param input Pointer to the InputType instance to remove.
     */
    void removeInput(InputType *input)
    {
        _inputs.erase(
            std::remove(_inputs.begin(), _inputs.end(), input),
            _inputs.end());
    }

    /**
     * @brief Get all InputTypes in this group.
     * @return Const reference to the internal vector of InputType pointers.
     */
    const std::vector<InputType *> &getInputs() const
    {
        return _inputs;
    }

    /**
     * @brief Check if a specific InputType is in this group.
     * @param input Pointer to InputType.
     * @return true if the input is part of the group.
     */
    bool contains(InputType *input) const
    {
        for (auto *i : _inputs)
        {
            if (i == input)
                return true;
        }
        return false;
    }

    /**
     * @brief Get the number of inputs in the group.
     * @return size_t Number of inputs.
     */
    size_t size() const
    {
        return _inputs.size();
    }

private:
    std::vector<InputType *> _inputs; ///< Pointers to InputType instances in this group
};
