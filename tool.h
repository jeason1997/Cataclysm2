#ifndef _TOOL_H_
#define _TOOL_H_

#include "dice.h"
#include "field.h"
#include "skill.h"  // Some actions are affected by skill
#include <string>
#include <istream>

// Tool_target is a list of ways that tools can select their target
enum Tool_target
{
  TOOL_TARGET_NULL = 0, // Default - doesn't use a target
  TOOL_TARGET_ADJACENT, // "adjacent" - press a direction key, apply there
  TOOL_TARGET_RANGED,   // "ranged" - target any square
  TOOL_TARGET_ALL,      // "all" - target EVERYTHING that's within our range
  TOOL_TARGET_MAX
};

Tool_target lookup_tool_target(std::string name);
std::string tool_target_name(Tool_target target);

enum Tool_special_type
{
  TOOL_SPECIAL_NULL = 0,  // Default - don't do anything
  TOOL_SPECIAL_EXPLOSION, // Make an explosion
  TOOL_SPECIAL_LIGHT,     // Provide some light
  TOOL_SPECIAL_HEAL,      // Heal some damage
  TOOL_SPECIAL_MAX
};

class Entity;
class Item;

struct Tool_special
{
  virtual ~Tool_special(){}

  virtual Tool_special_type get_type() { return TOOL_SPECIAL_NULL; }

  virtual bool load_data(std::istream& data, std::string owner_name)
  {
    return false;
  }

  virtual bool effect(Entity* user) { return false; }
};

struct Tool_special_explosion : public Tool_special
{
  Tool_special_explosion();
  virtual ~Tool_special_explosion(){}

  virtual Tool_special_type get_type() { return TOOL_SPECIAL_EXPLOSION; }

  virtual bool load_data(std::istream& data, std::string owner_name);
  virtual bool effect(Entity* user) { return false; }; // TODO: This

  Dice damage;
  Dice radius;
// TODO: Field output, shrapnel, shrapnel damage, other effects (flashbang), etc
};

struct Tool_special_light : public Tool_special
{
  Tool_special_light();
  virtual ~Tool_special_light(){}

  virtual Tool_special_type get_type() { return TOOL_SPECIAL_LIGHT; }

  virtual bool load_data(std::istream& data, std::string owner_name);
  virtual bool effect(Entity* user);  // TODO: This

  int light;
};

struct Tool_special_heal : public Tool_special
{
  Tool_special_heal();
  virtual ~Tool_special_heal(){}

  virtual Tool_special_type get_type() { return TOOL_SPECIAL_HEAL; }

  virtual bool load_data(std::istream& data, std::string owner_name);
  virtual bool effect(Entity* user);

  int min_amount, max_amount;
// min_skill is the minimum requirement; max_skill is what's required to reach
// max_amount.
  Skill_type skill;
  int skill_min, skill_max;
};

struct Tool_action
{
  Tool_action();
  ~Tool_action();

  std::string signal;     // Signal sent to terrain/items/entities
  Tool_special *special;  // Hardcoded action
  Tool_target target;     // The target it's sent to
  Field_pool field;       // A field we drop
  int ap_cost;            // Action Points used
  int charge_cost;        // Charges used
  int range;              // Range for TOOL_TARGET_RANGED, TOOL_TARGET_ALL, etc
  bool destroy_if_chargeless; // Destroy the item if all charges used.
  bool real;              // If true, then this is in use

  bool load_data(std::istream& data, std::string owner_name = "Unknown");

  bool activate(Item* it);  // Uses NULL user
// Uses Game::find_item to create pos for activate() below
  bool activate(Item* it, Entity* user); 
// Send signal, create field, activate special!
  bool activate(Item* it, Entity* user, Tripoint pos);
};
  
#endif
