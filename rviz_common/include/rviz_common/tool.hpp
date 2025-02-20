/*
 * Copyright (c) 2012, Willow Garage, Inc.
 * Copyright (c) 2017, Open Source Robotics Foundation, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Willow Garage, Inc. nor the names of its
 *       contributors may be used to endorse or promote products derived from
 *       this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef RVIZ_COMMON__TOOL_HPP_
#define RVIZ_COMMON__TOOL_HPP_

#include <QCursor>
#include <QIcon>
#include <QObject>
#include <QString>

#include "rviz_common/config.hpp"
#include "rviz_common/visibility_control.hpp"

class QKeyEvent;

namespace Ogre
{
class SceneManager;
}  // namespace Ogre

namespace rviz_common
{

namespace properties
{

class Property;

}  // namespace properties

class DisplayContext;
class RenderPanel;
class ViewportMouseEvent;

class RVIZ_COMMON_PUBLIC Tool : public QObject
{
  Q_OBJECT

public:
  /**
   * Pluginlib only instantiates classes via default constructors.
   * Subclasses of Tool should set the shortcut_key_ field in their
   * constructors.
   *
   * Properties to appear in the Tool Properties panel are typically
   * created in the constructor, as children of the property from
   * getPropertyContainer(), which is set up in this Tool
   * constructor.
   */
  Tool();

  ~Tool() override;

  /// Initialize the tool.
  /**
   * Sets the DisplayContext and calls onInitialize().
   */
  void initialize(DisplayContext * context);

  /// Return the container for properties of this Tool.
  virtual rviz_common::properties::Property * getPropertyContainer() const;

  /// Get the shortcut key for the tool.
  char getShortcutKey() const;

  /// Return true if the tool needs to access all keys or false if not.
  bool accessAllKeys() const;

  /// Override to get called when the tool is activated.
  virtual void activate() = 0;

  /// Override to get called when the tool is deactivated.
  virtual void deactivate() = 0;

  /// Called periodically, typically at 30Hz.
  virtual void update(float wall_dt, float ros_dt);

  enum
  {
    Render = 1,
    Finished = 2
  };

  /// Process a mouse event.
  /**
   * This is the central function of all the tools, as it defines how the
   * mouse is used.
   */
  virtual int processMouseEvent(ViewportMouseEvent & event);

  /// Process a key event.
  /**
    * Override if your tool should handle any other keypresses than the tool
    * shortcuts, which are handled separately.
    */
  virtual int processKeyEvent(QKeyEvent * event, RenderPanel * panel);

  /// Get the name of the tool.
  QString getName() const;

  /// Set the name of the tool.
  /**
   * This is called by ToolManager during tool initialization.
   * If you want a different name than it gives you, call this from
   * onInitialize() (or thereafter).
   */
  void setName(const QString & name);

  /// Get the description.
  QString getDescription() const;

  /// Set the description of the tool.
  /**
   * This is called by ToolManager during tool initialization.
   */
  void setDescription(const QString & description);

  /// Return the class identifier which was used to create this instance.
  /**
   * This version just returns whatever was set with setClassId().
   */
  virtual QString getClassId() const;

  /// Set the class identifier used to create this instance.
  /**
   * Typically this will be set by the factory object which created it.
   */
  virtual void setClassId(const QString & class_id);

  /// Load properties from the given Config.
  /**
   * Most tools won't need to override this, because any child
   * Properties of property_container_ are automatically loaded by
   * this function.
   */
  virtual void load(const Config & config);

  /// Save this entire tool into the given Config node.
  /**
   * Most tools won't need to override this, because any child
   * Properties of property_container_ are automatically saved by
   * this function.
   */
  virtual void save(Config config) const;

  /// Set the toolbar icon for this tool (will also set its cursor).
  void setIcon(const QIcon & icon);

  /// Get the icon of this tool.
  const QIcon & getIcon();

  /// Set the cursor for this tool.
  void setCursor(const QCursor & cursor);

  /// Get current cursor of this tool.
  const QCursor & getCursor();

  /// Set the status message.
  void setStatus(const QString & message);

Q_SIGNALS:
  /// Emitted when closed.
  void close();
  /// Emitted when name property has been changed.
  void nameChanged(const QString& name);

protected:
  /// Override onInitialize to do any setup needed after the DisplayContext has been set.
  /**
   * This is called by Tool::initialize().
   */
  virtual void onInitialize() {}

  Ogre::SceneManager * scene_manager_;
  DisplayContext * context_;

  char shortcut_key_;
  bool access_all_keys_;

  QIcon icon_;

  QCursor cursor_;

private:
  QString class_id_;
  rviz_common::properties::Property * property_container_;
  QString name_;
  QString description_;
};

}  // namespace rviz_common

#endif  // RVIZ_COMMON__TOOL_HPP_
