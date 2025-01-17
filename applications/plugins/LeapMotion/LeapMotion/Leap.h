/******************************************************************************\
* Copyright (C) 2012-2013 Leap Motion, Inc. All rights reserved.               *
* Leap Motion proprietary and confidential. Not for distribution.              *
* Use subject to the terms of the Leap Motion SDK Agreement available at       *
* https://developer.leapmotion.com/sdk_agreement, or another agreement         *
* between Leap Motion and you, your company or other organization.             *
\******************************************************************************/

#if !defined(__Leap_h__)
#define __Leap_h__

#include "LeapMath.h"

#include <string>
#include <vector>

// Define integer types for Visual Studio 2008 and earlier
#if defined(_MSC_VER) && (_MSC_VER < 1600)
typedef __int32 int32_t;
typedef __int64 int64_t;
typedef unsigned __int32 uint32_t;
typedef unsigned __int64 uint64_t;
#else
#include <stdint.h>
#endif

// Define Leap export macros
#if defined(_WIN32) // Windows
#if LEAP_API_INTERNAL
#define LEAP_EXPORT
#elif LEAP_API_IMPLEMENTATION
#define LEAP_EXPORT __declspec(dllexport)
#else
#define LEAP_EXPORT __declspec(dllimport)
#endif
#define LEAP_EXPORT_CLASS
#define LEAP_EXPORT_PLUGIN __declspec(dllexport)
#elif !defined(SWIG)
#define LEAP_EXPORT __attribute__((visibility("default")))
#define LEAP_EXPORT_CLASS __attribute__((visibility("default")))
#define LEAP_EXPORT_PLUGIN __attribute__((visibility("default")))
#else
#define LEAP_EXPORT
#define LEAP_EXPORT_CLASS
#define LEAP_EXPORT_PLUGIN
#endif

namespace Leap {

// Interface for internal use only
class LEAP_EXPORT_CLASS Interface {
  public:
    struct Implementation {
      LEAP_EXPORT virtual ~Implementation() {}
    };
  protected:
    LEAP_EXPORT Interface(void* owner);
    LEAP_EXPORT Interface(Implementation* reference, void* owner);
    LEAP_EXPORT Interface(const Interface& rhs);
    LEAP_EXPORT Interface& operator=(const Interface& rhs);
    LEAP_EXPORT virtual ~Interface();
    template<typename T> T* get() const { return static_cast<T*>(reference()); }
    class SharedObject* m_object;
  private:
    LEAP_EXPORT Implementation* reference() const;
};

// Forward declarations for internal use only
class PointableImplementation;
class FingerImplementation;
class ToolImplementation;
class HandImplementation;
class GestureImplementation;
class ScreenImplementation;
class FrameImplementation;
class ControllerImplementation;
template<typename T> class ListBaseImplementation;

// Forward declarations
class PointableList;
class FingerList;
class ToolList;
class HandList;
class GestureList;
class Hand;
class Gesture;
class Screen;
class Frame;
class Listener;

/**
 * The Pointable class reports the physical characteristics of a detected finger or tool.
 *
 * Both fingers and tools are classified as Pointable objects. Use the Pointable::isFinger()
 * function to determine whether a Pointable object represents a finger. Use the
 * Pointable::isTool() function to determine whether a Pointable object represents a tool.
 * The Leap classifies a detected entity as a tool when it is thinner, straighter, and longer
 * than a typical finger.
 *
 * Note that Pointable objects can be invalid, which means that they do not contain
 * valid tracking data and do not correspond to a physical entity. Invalid Pointable
 * objects can be the result of asking for a Pointable object using an ID from an
 * earlier frame when no Pointable objects with that ID exist in the current frame.
 * A Pointable object created from the Pointable constructor is also invalid.
 * Test for validity with the Pointable::isValid() function.
 */
class Pointable : public Interface {
  public:
    // For internal use only.
    Pointable(PointableImplementation*);
    // For internal use only.
    Pointable(FingerImplementation*);
    // For internal use only.
    Pointable(ToolImplementation*);

    /**
     * Constructs a Pointable object.
     *
     * An uninitialized pointable is considered invalid.
     * Get valid Pointable objects from a Frame or a Hand object.
     */
    LEAP_EXPORT Pointable();

    /**
     * A unique ID assigned to this Pointable object, whose value remains the
     * same across consecutive frames while the tracked finger or tool remains
     * visible. If tracking is lost (for example, when a finger is occluded by
     * another finger or when it is withdrawn from the Leap field of view), the
     * Leap may assign a new ID when it detects the entity in a future frame.
     *
     * Use the ID value with the Frame::pointable() function to find this
     * Pointable object in future frames.
     *
     * @returns The ID assigned to this Pointable object.
     */
    LEAP_EXPORT int32_t id() const;

    /**
     * The Frame associated with this Pointable object.
     *
     * @returns The associated Frame object, if available; otherwise,
     * an invalid Frame object is returned.
     */
    LEAP_EXPORT Frame frame() const;

    /**
     * The Hand associated with this finger or tool.
     *
     * @returns The associated Hand object, if available; otherwise,
     * an invalid Hand object is returned.
     */
    LEAP_EXPORT Hand hand() const;

    /**
     * The tip position in millimeters from the Leap origin.
     *
     * @returns The Vector containing the coordinates of the tip position.
     */
    LEAP_EXPORT Vector tipPosition() const;

    /**
     * The rate of change of the tip position in millimeters/second.
     *
     * @returns The Vector containing the coordinates of the tip velocity.
     */
    LEAP_EXPORT Vector tipVelocity() const;

    /**
     * The direction in which this finger or tool is pointing.
     *
     * The direction is expressed as a unit vector pointing in the same
     * direction as the tip.
     *
     * \image html images/Leap_Finger_Model.png
     *
     * @returns The Vector pointing in the same direction as the tip of this
     * Pointable object.
     */
    LEAP_EXPORT Vector direction() const;

    /**
     * The estimated width of the finger or tool in millimeters.
     *
     * The reported width is the average width of the visible portion of the
     * finger or tool from the hand to the tip. If the width isn't known,
     * then a value of 0 is returned.
     *
     * @returns The estimated width of this Pointable object.
     */
    LEAP_EXPORT float width() const;

    /**
     * The estimated length of the finger or tool in millimeters.
     *
     * The reported length is the visible length of the finger or tool from the
     * hand to tip. If the length isn't known, then a value of 0 is returned.
     *
     * @returns The estimated length of this Pointable object.
     */
    LEAP_EXPORT float length() const;

    /**
     * Whether or not the Pointable is believed to be a finger.
     * Fingers are generally shorter, thicker, and less straight than tools.
     *
     * @returns True, if this Pointable is classified as a finger.
     */
    LEAP_EXPORT bool isFinger() const;

    /**
     * Whether or not the Pointable is believed to be a tool.
     * Tools are generally longer, thinner, and straighter than fingers.
     *
     * @returns True, if this Pointable is classified as a tool.
     */
    LEAP_EXPORT bool isTool() const;

    /**
     * Reports whether this is a valid Pointable object.
     *
     * @returns True, if this Pointable object contains valid tracking data.
     */
    LEAP_EXPORT bool isValid() const;

    /**
     * Returns an invalid Pointable object.
     *
     * You can use the instance returned by this function in comparisons testing
     * whether a given Pointable instance is valid or invalid. (You can also use the
     * Pointable::isValid() function.)
     *
     * @returns The invalid Pointable instance.
     */
    LEAP_EXPORT static const Pointable& invalid();

    /**
     * Compare Pointable object equality.
     * Two Pointable objects are equal if and only if both Pointable objects represent the
     * exact same physical entities in the same frame and both Pointable objects are valid.
     */
    LEAP_EXPORT bool operator==(const Pointable&) const;

    /**
     * Compare Pointable object inequality.
     * Two Pointable objects are equal if and only if both Pointable objects represent the
     * exact same physical entities in the same frame and both Pointable objects are valid.
     */
    LEAP_EXPORT bool operator!=(const Pointable&) const;

    /** Writes a brief, human readable description of the Pointable object to an output stream. */
    LEAP_EXPORT friend std::ostream& operator<<(std::ostream&, const Pointable&);

    /**
     * A string containing a brief, human readable description of the Pointable object.
     *
     * @returns A description of the Pointable object as a string.
     */
    LEAP_EXPORT std::string toString() const;
};

/**
 * The Finger class represents a tracked finger.
 *
 * Fingers are Pointable objects that the Leap has classified as a finger.
 * Get valid Finger objects from a Frame or a Hand object.
 *
 * Note that Finger objects can be invalid, which means that they do not contain
 * valid tracking data and do not correspond to a physical finger. Invalid Finger
 * objects can be the result of asking for a Finger object using an ID from an
 * earlier frame when no Finger objects with that ID exist in the current frame.
 * A Finger object created from the Finger constructor is also invalid.
 * Test for validity with the Finger::isValid() function.
 */
class Finger : public Pointable {
  public:
    // For internal use only.
    Finger(FingerImplementation*);

    /**
     * Constructs a Finger object.
     *
     * An uninitialized finger is considered invalid.
     * Get valid Finger objects from a Frame or a Hand object.
     */
    LEAP_EXPORT Finger();

    /**
     * If the specified Pointable object represents a finger, creates a copy
     * of it as a Finger object; otherwise, creates an invalid Finger object.
     */
    LEAP_EXPORT explicit Finger(const Pointable&);

    /**
     * Returns an invalid Finger object.
     *
     * You can use the instance returned by this function in comparisons testing
     * whether a given Finger instance is valid or invalid. (You can also use the
     * Finger::isValid() function.)
     *
     * @returns The invalid Finger instance.
     */
    LEAP_EXPORT static const Finger& invalid();

    /**
     * A string containing a brief, human readable description of the Finger object.
     *
     * @returns A description of the Finger object as a string.
     */
    LEAP_EXPORT std::string toString() const;
};

/**
 * The Tool class represents a tracked tool.
 *
 * Tools are Pointable objects that the Leap has classified as a tool.
 * Tools are longer, thinner, and straighter than a typical finger.
 * Get valid Tool objects from a Frame or a Hand object.
 *
 * \image html images/Leap_Tool.png
 *
 * Note that Tool objects can be invalid, which means that they do not contain
 * valid tracking data and do not correspond to a physical tool. Invalid Tool
 * objects can be the result of asking for a Tool object using an ID from an
 * earlier frame when no Tool objects with that ID exist in the current frame.
 * A Tool object created from the Tool constructor is also invalid.
 * Test for validity with the Tool::isValid() function.
 */
class Tool : public Pointable {
  public:
    // For internal use only.
    Tool(ToolImplementation*);

    /**
     * Constructs a Tool object.
     *
     * An uninitialized tool is considered invalid.
     * Get valid Tool objects from a Frame or a Hand object.
     */
    LEAP_EXPORT Tool();

    /**
     * If the specified Pointable object represents a tool, creates a copy
     * of it as a Tool object; otherwise, creates an invalid Tool object.
     */
    LEAP_EXPORT explicit Tool(const Pointable&);

    /**
     * Returns an invalid Tool object.
     *
     * You can use the instance returned by this function in comparisons testing
     * whether a given Tool instance is valid or invalid. (You can also use the
     * Tool::isValid() function.)
     *
     * @returns The invalid Tool instance.
     */
    LEAP_EXPORT static const Tool& invalid();

    /**
     * A string containing a brief, human readable description of the Tool object.
     *
     * @returns A description of the Tool object as a string.
     */
    LEAP_EXPORT std::string toString() const;
};

/**
 * The Hand class reports the physical characteristics of a detected hand.
 *
 * Hand tracking data includes a palm position and velocity; vectors for
 * the palm normal and direction to the fingers; properties of a sphere fit
 * to the hand; and lists of the attached fingers and tools.
 *
 * Note that Hand objects can be invalid, which means that they do not contain
 * valid tracking data and do not correspond to a physical entity. Invalid Hand
 * objects can be the result of asking for a Hand object using an ID from an
 * earlier frame when no Hand objects with that ID exist in the current frame.
 * A Hand object created from the Hand constructor is also invalid.
 * Test for validity with the Hand::isValid() function.
 */
class Hand : public Interface {
  public:
    // For internal use only.
    Hand(HandImplementation*);

    /**
     * Constructs a Hand object.
     *
     * An uninitialized hand is considered invalid.
     * Get valid Hand objects from a Frame object.
     */
    LEAP_EXPORT Hand();

    /**
     * A unique ID assigned to this Hand object, whose value remains the same
     * across consecutive frames while the tracked hand remains visible. If
     * tracking is lost (for example, when a hand is occluded by another hand
     * or when it is withdrawn from or reaches the edge of the Leap field of view),
     * the Leap may assign a new ID when it detects the hand in a future frame.
     *
     * Use the ID value with the Frame::hand() function to find this Hand object
     * in future frames.
     *
     * @returns The ID of this hand.
     */
    LEAP_EXPORT int32_t id() const;

    /**
     * The Frame associated with this Hand.
     *
     * @returns The associated Frame object, if available; otherwise,
     * an invalid Frame object is returned.
     */
    LEAP_EXPORT Frame frame() const;

    /**
     * The list of Pointable objects (fingers and tools) detected in this frame
     * that are associated with this hand, given in arbitrary order. The list
     * can be empty if no fingers or tools associated with this hand are detected.
     *
     * Use the Pointable::isFinger() function to determine whether or not an
     * item in the list represents a finger. Use the Pointable::isTool() function
     * to determine whether or not an item in the list represents a tool.
     * You can also get only fingers using the Hand::fingers() function or
     * only tools using the Hand::tools() function.
     *
     * @returns The PointableList containing all Pointable objects associated with this hand.
     */
    LEAP_EXPORT PointableList pointables() const;

    /**
     * The Pointable object with the specified ID associated with this hand.
     *
     * Use the Hand::pointable() function to retrieve a Pointable object
     * associated with this hand using an ID value obtained from a previous frame.
     * This function always returns a Pointable object, but if no finger or tool
     * with the specified ID is present, an invalid Pointable object is returned.
     *
     * Note that ID values persist across frames, but only until tracking of a
     * particular object is lost. If tracking of a finger or tool is lost and subsequently
     * regained, the new Pointable object representing that finger or tool may have a
     * different ID than that representing the finger or tool in an earlier frame.
     *
     * @param id The ID value of a Pointable object from a previous frame.
     * @returns The Pointable object with the matching ID if one exists for this
     * hand in this frame; otherwise, an invalid Pointable object is returned.
     */
    LEAP_EXPORT Pointable pointable(int32_t id) const;

    /**
     * The list of Finger objects detected in this frame that are attached to
     * this hand, given in arbitrary order.
     * The list can be empty if no fingers attached to this hand are detected.
     *
     * @returns The FingerList containing all Finger objects attached to this hand.
     */
    LEAP_EXPORT FingerList fingers() const;

    /**
     * The Finger object with the specified ID attached to this hand.
     *
     * Use the Hand::finger() function to retrieve a Finger object attached to
     * this hand using an ID value obtained from a previous frame.
     * This function always returns a Finger object, but if no finger
     * with the specified ID is present, an invalid Finger object is returned.
     *
     * Note that ID values persist across frames, but only until tracking of a
     * particular object is lost. If tracking of a finger is lost and subsequently
     * regained, the new Finger object representing that finger may have a
     * different ID than that representing the finger in an earlier frame.
     *
     * @param id The ID value of a Finger object from a previous frame.
     * @returns The Finger object with the matching ID if one exists for this
     * hand in this frame; otherwise, an invalid Finger object is returned.
     */
    LEAP_EXPORT Finger finger(int32_t id) const;

    /**
     * The list of Tool objects detected in this frame that are held by this
     * hand, given in arbitrary order.
     * The list can be empty if no tools held by this hand are detected.
     *
     * @returns The ToolList containing all Tool objects held by this hand.
     */
    LEAP_EXPORT ToolList tools() const;

    /**
     * The Tool object with the specified ID held by this hand.
     *
     * Use the Hand::tool() function to retrieve a Tool object held by
     * this hand using an ID value obtained from a previous frame.
     * This function always returns a Tool object, but if no tool
     * with the specified ID is present, an invalid Tool object is returned.
     *
     * Note that ID values persist across frames, but only until tracking of a
     * particular object is lost. If tracking of a tool is lost and subsequently
     * regained, the new Tool object representing that tool may have a
     * different ID than that representing the tool in an earlier frame.
     *
     * @param id The ID value of a Tool object from a previous frame.
     * @returns The Tool object with the matching ID if one exists for this
     * hand in this frame; otherwise, an invalid Tool object is returned.
     */
    LEAP_EXPORT Tool tool(int32_t id) const;

    /**
     * The center position of the palm in millimeters from the Leap origin.
     *
     * @returns The Vector representing the coordinates of the palm position.
     */
    LEAP_EXPORT Vector palmPosition() const;

    /**
     * The rate of change of the palm position in millimeters/second.
     *
     * @returns The Vector representing the coordinates of the palm velocity.
     */
    LEAP_EXPORT Vector palmVelocity() const;

    /**
     * The normal vector to the palm. If your hand is flat, this vector will
     * point downward, or "out" of the front surface of your palm.
     *
     * \image html images/Leap_Palm_Vectors.png
     *
     * The direction is expressed as a unit vector pointing in the same
     * direction as the palm normal (that is, a vector orthogonal to the palm).
     *
     * @returns The Vector normal to the plane formed by the palm.
     */
    LEAP_EXPORT Vector palmNormal() const;

    /**
     * The direction from the palm position toward the fingers.
     *
     * The direction is expressed as a unit vector pointing in the same
     * direction as the directed line from the palm position to the fingers.
     *
     * @returns The Vector pointing from the palm position toward the fingers.
     */
    LEAP_EXPORT Vector direction() const;

    /**
     * The center of a sphere fit to the curvature of this hand.
     *
     * This sphere is placed roughly as if the hand were holding a ball.
     *
     * \image html images/Leap_Hand_Ball.png
     *
     * @returns The Vector representing the center position of the sphere.
     */
    LEAP_EXPORT Vector sphereCenter() const;

    /**
     * The radius of a sphere fit to the curvature of this hand.
     *
     * This sphere is placed roughly as if the hand were holding a ball. Thus the
     * size of the sphere decreases as the fingers are curled into a fist.
     * @returns The radius of the sphere in millimeters.
     */
    LEAP_EXPORT float sphereRadius() const;

    /**
     * The change of position of this hand between the current frame and
     * the specified frame.
     *
     * The returned translation vector provides the magnitude and direction of
     * the movement in millimeters.
     *
     * If a corresponding Hand object is not found in sinceFrame, or if either
     * this frame or sinceFrame are invalid Frame objects, then this method
     * returns a zero vector.
     *
     * @param sinceFrame The starting frame for computing the translation.
     * @returns A Vector representing the heuristically determined change in
     * hand position between the current frame and that specified in the
     * sinceFrame parameter.
     */
    LEAP_EXPORT Vector translation(const Frame& sinceFrame) const;

    /**
     * The estimated probability that the hand motion between the current
     * frame and the specified frame is intended to be a translating motion.
     *
     * If a corresponding Hand object is not found in sinceFrame, or if either
     * this frame or sinceFrame are invalid Frame objects, then this method
     * returns zero.
     *
     * @param sinceFrame The starting frame for computing the translation.
     * @returns A value between 0 and 1 representing the estimated probability
     * that the hand motion between the current frame and the specified frame
     * is intended to be a translating motion.
     */
    LEAP_EXPORT float translationProbability(const Frame& sinceFrame) const;

    /**
     * The axis of rotation derived from the change in orientation of this
     * hand, and any associated fingers and tools, between the current frame
     * and the specified frame.
     *
     * The returned direction vector is normalized.
     *
     * If a corresponding Hand object is not found in sinceFrame, or if either
     * this frame or sinceFrame are invalid Frame objects, then this method
     * returns a zero vector.
     *
     * @param sinceFrame The starting frame for computing the relative rotation.
     * @returns A normalized direction Vector representing the heuristically
     * determined axis of rotational change of the hand between the current
     * frame and that specified in the sinceFrame parameter.
     */
    LEAP_EXPORT Vector rotationAxis(const Frame& sinceFrame) const;

    /**
     * The angle of rotation around the rotation axis derived from the change
     * in orientation of this hand, and any associated fingers and tools,
     * between the current frame and the specified frame.
     *
     * The returned angle is expressed in radians measured clockwise around the
     * rotation axis (using the right-hand rule) between the start and end frames.
     * The value is always between 0 and pi radians (0 and 180 degrees).
     *
     * If a corresponding Hand object is not found in sinceFrame, or if either
     * this frame or sinceFrame are invalid Frame objects, then the angle of
     * rotation is zero.
     *
     * @param sinceFrame The starting frame for computing the relative rotation.
     * @returns A positive value representing the heuristically determined
     * rotational change of the hand between the current frame and that
     * specified in the sinceFrame parameter.
     */
    LEAP_EXPORT float rotationAngle(const Frame& sinceFrame) const;

    /**
     * The angle of rotation around the specified axis derived from the change
     * in orientation of this hand, and any associated fingers and tools,
     * between the current frame and the specified frame.
     *
     * The returned angle is expressed in radians measured clockwise around the
     * rotation axis (using the right-hand rule) between the start and end frames.
     * The value is always between -pi and pi radians (-180 and 180 degrees).
     *
     * If a corresponding Hand object is not found in sinceFrame, or if either
     * this frame or sinceFrame are invalid Frame objects, then the angle of
     * rotation is zero.
     *
     * @param sinceFrame The starting frame for computing the relative rotation.
     * @param axis The axis to measure rotation around.
     * @returns A value representing the heuristically determined rotational
     * change of the hand between the current frame and that specified in the
     * sinceFrame parameter around the specified axis.
     */
    LEAP_EXPORT float rotationAngle(const Frame& sinceFrame, const Vector& axis) const;

    /**
     * The transform matrix expressing the rotation derived from the change
     * in orientation of this hand, and any associated fingers and tools,
     * between the current frame and the specified frame.
     *
     * If a corresponding Hand object is not found in sinceFrame, or if either
     * this frame or sinceFrame are invalid Frame objects, then this method
     * returns an identity matrix.
     *
     * @param sinceFrame The starting frame for computing the relative rotation.
     * @returns A transformation Matrix representing the heuristically determined
     * rotational change of the hand between the current frame and that specified
     * in the sinceFrame parameter.
     */
    LEAP_EXPORT Matrix rotationMatrix(const Frame& sinceFrame) const;

    /**
     * The estimated probability that the hand motion between the current
     * frame and the specified frame is intended to be a rotating motion.
     *
     * If a corresponding Hand object is not found in sinceFrame, or if either
     * this frame or sinceFrame are invalid Frame objects, then this method
     * returns zero.
     *
     * @param sinceFrame The starting frame for computing the relative rotation.
     * @returns A value between 0 and 1 representing the estimated probability
     * that the hand motion between the current frame and the specified frame
     * is intended to be a rotating motion.
     */
    LEAP_EXPORT float rotationProbability(const Frame& sinceFrame) const;

    /**
     * The scale factor derived from this hand's motion between the current frame
     * and the specified frame.
     *
     * The scale factor is always positive. A value of 1.0 indicates no
     * scaling took place. Values between 0.0 and 1.0 indicate contraction
     * and values greater than 1.0 indicate expansion.
     *
     * The Leap derives scaling from the relative inward or outward motion of
     * a hand and its associated fingers and tools (independent of translation
     * and rotation).
     *
     * If a corresponding Hand object is not found in sinceFrame, or if either
     * this frame or sinceFrame are invalid Frame objects, then this method
     * returns 1.0.
     *
     * @param sinceFrame The starting frame for computing the relative scaling.
     * @returns A positive value representing the heuristically determined
     * scaling change ratio of the hand between the current frame and that
     * specified in the sinceFrame parameter.
     */
    LEAP_EXPORT float scaleFactor(const Frame& sinceFrame) const;

    /**
     * The estimated probability that the hand motion between the current
     * frame and the specified frame is intended to be a scaling motion.
     *
     * If a corresponding Hand object is not found in sinceFrame, or if either
     * this frame or sinceFrame are invalid Frame objects, then this method
     * returns zero.
     *
     * @param sinceFrame The starting frame for computing the relative scaling.
     * @returns A value between 0 and 1 representing the estimated probability
     * that the hand motion between the current frame and the specified frame
     * is intended to be a scaling motion.
     */
    LEAP_EXPORT float scaleProbability(const Frame& sinceFrame) const;

    /**
     * Reports whether this is a valid Hand object.
     *
     * @returns True, if this Hand object contains valid tracking data.
     */
    LEAP_EXPORT bool isValid() const;

    /**
     * Returns an invalid Hand object.
     *
     * You can use the instance returned by this function in comparisons testing
     * whether a given Hand instance is valid or invalid. (You can also use the
     * Hand::isValid() function.)
     *
     * @returns The invalid Hand instance.
     */
    LEAP_EXPORT static const Hand& invalid();

    /**
     * Compare Hand object equality.
     * Two Hand objects are equal if and only if both Hand objects represent the
     * exact same physical hand in the same frame and both Hand objects are valid.
     */
    LEAP_EXPORT bool operator==(const Hand&) const;

    /**
     * Compare Hand object inequality.
     * Two Hand objects are equal if and only if both Hand objects represent the
     * exact same physical hand in the same frame and both Hand objects are valid.
     */
    LEAP_EXPORT bool operator!=(const Hand&) const;

    /** Writes a brief, human readable description of the Hand object to an output stream. */
    LEAP_EXPORT friend std::ostream& operator<<(std::ostream&, const Hand&);

    /**
     * A string containing a brief, human readable description of the Hand object.
     *
     * @returns A description of the Hand as a string.
     */
    LEAP_EXPORT std::string toString() const;
};

/**
 * The Gesture class represents a recognized movement by the user.
 *
 * The Leap watches the activity within its field of view for certain movement
 * patterns typical of a user gesture or command. For example, a movement from side to
 * side with the hand can indicate a swipe gesture, while a finger poking forward
 * can indicate a screen tap gesture.
 *
 * When the Leap recognizes a gesture, it assigns an ID and adds a
 * Gesture object to the frame gesture list. For continuous gestures, which
 * occur over many frames, the Leap updates the gesture by adding
 * a Gesture object having the same ID and updated properties in each
 * subsequent frame.
 *
 * **Important:** Recognition for each type of gesture must be enabled using the
 * Controller::enableGesture() function; otherwise **no gestures are recognized or
 * reported**.
 *
 * Subclasses of Gesture define the properties for the specific movement patterns
 * recognized by the Leap.
 *
 * The Gesture subclasses for include:
 *
 * * CircleGesture -- A circular movement by a finger.
 * * SwipeGesture -- A straight line movement by the hand with fingers extended.
 * * ScreenTapGesture -- A forward tapping movement by a finger.
 * * KeyTapGesture -- A downward tapping movement by a finger.
 *
 * Circle and swipe gestures are continuous and these objects can have a
 * state of start, update, and stop.
 *
 * The screen tap gesture is a discrete gesture. The Leap only creates a single
 * ScreenTapGesture object appears for each tap and it always has a stop state.
 *
 * Get valid Gesture instances from a Frame object. You can get a list of gestures
 * with the Frame::gestures() method. You can get a list of gestures since a
 * specified frame with the `Frame::gestures(const Frame&)` method. You can also
 * use the `Frame::gesture()` method to find a gesture in the current frame using
 * an ID value obtained in a previous frame.
 *
 * Gesture objects can be invalid. For example, when you get a gesture by ID
 * using `Frame::gesture()`, and there is no gesture with that ID in the current
 * frame, then `gesture()` returns an Invalid Gesture object (rather than a null
 * value). Always check object validity in situations where a gesture might be
 * invalid.
 * 
 * The following keys can be used with the Config class to configure the gesture 
 * recognizer:
 * 
 * Key string | Value type | Default value | Units
 * -----------|------------|---------------|------
 * Gesture.Circle.MinRadius | float | 5.0 | mm
 * Gesture.Circle.MinArc | float | 1.5*pi | radians
 * Gesture.Swipe.MinLength | float | 150 | mm
 * Gesture.Swipe.MinVelocity | float | 1000 | mm/s
 * Gesture.KeyTap.MinDownVelocity | float | 50 | mm/s
 * Gesture.KeyTap.HistorySeconds | float | 0.1 | s
 * Gesture.KeyTap.MinDistance | float | 5.0 | mm
 * Gesture.ScreenTap.MinForwardVelocity  | float | 50 | mm/s
 * Gesture.ScreenTap.HistorySeconds | float | 0.1 | s
 * Gesture.ScreenTap.MinDistance | float | 3.0 | mm
 */
class Gesture : public Interface {
  public:
    // For internal use only.
    Gesture(GestureImplementation*);

    /**
     * The supported types of gestures.
     */
    enum Type {
      TYPE_INVALID    = -1, /**< An invalid type. */
      TYPE_SWIPE      = 1,  /**< A straight line movement by the hand with fingers extended. */
      TYPE_CIRCLE     = 4,  /**< A circular movement by a finger. */
      TYPE_SCREEN_TAP = 5,  /**< A forward tapping movement by a finger. */
      TYPE_KEY_TAP    = 6   /**< A downward tapping movement by a finger. */
    };

    /**
     * The possible gesture states.
     */
    enum State {
      STATE_INVALID = -1, /**< An invalid state */
      STATE_START   = 1,  /**< The gesture is starting. Just enough has happened to recognize it. */
      STATE_UPDATE  = 2,  /**< The gesture is in progress. (Note: not all gestures have updates). */
      STATE_STOP    = 3,  /**< The gesture has completed or stopped. */
    };

    /**
     * Constructs a new Gesture object.
     *
     * An uninitialized Gesture object is considered invalid. Get valid instances
     * of the Gesture class, which will be one of the Gesture subclasses, from a
     * Frame object.
     */
    LEAP_EXPORT Gesture();

    /**
     * Constructs a new copy of an Gesture object.
     */
    LEAP_EXPORT Gesture(const Gesture& rhs);

    /**
     * The gesture type.
     *
     * @returns Gesture::Type A value from the Gesture::Type enumeration.
     */
    LEAP_EXPORT Type type() const;

    /**
     * The gesture state.
     *
     * Recognized movements occur over time and have a beginning, a middle,
     * and an end. The 'state()' attribute reports where in that sequence this
     * Gesture object falls.
     *
     * @returns Gesture::State A value from the Gesture::State enumeration.
     */
    LEAP_EXPORT State state() const;

    /**
     * The gesture ID.
     *
     * All Gesture objects belonging to the same recognized movement share the
     * same ID value. Use the ID value with the Frame::gesture() method to
     * find updates related to this Gesture object in subsequent frames.
     *
     * @returns int32_t the ID of this Gesture.
     */
    LEAP_EXPORT int32_t id() const;

    /**
     * The elapsed duration of the recognized movement up to the
     * frame containing this Gesture object, in microseconds.
     *
     * The duration reported for the first Gesture in the sequence (with the
     * STATE_START state) will typically be a small positive number since
     * the movement must progress far enough for the Leap to recognize it as
     * an intentional gesture.
     *
     * @return int64_t the elapsed duration in microseconds.
     */
    LEAP_EXPORT int64_t duration() const;

    /**
     * The elapsed duration in seconds.
     * @see duration()
     * @return float the elapsed duration in seconds.
     */
    LEAP_EXPORT float durationSeconds() const;

    /**
     * The Frame containing this Gesture instance.
     *
     * @return Frame The parent Frame object.
     */
    LEAP_EXPORT Frame frame() const;

    /**
     * The list of hands associated with this Gesture, if any.
     *
     * If no hands are related to this gesture, the list is empty.
     *
     * @return HandList the list of related Hand objects.
     */
    LEAP_EXPORT HandList hands() const;

    /**
     * The list of fingers and tools associated with this Gesture, if any.
     *
     * If no Pointable objects are related to this gesture, the list is empty.
     *
     * @return PointableList the list of related Pointable objects.
     */
    LEAP_EXPORT PointableList pointables() const;

    /**
     * Reports whether this Gesture instance represents a valid Gesture.
     *
     * An invalid Gesture object does not represent a snapshot of a recognized
     * movement. Invalid Gesture objects are returned when a valid object cannot
     * be provided. For example, when you get an gesture by ID
     * using Frame::gesture(), and there is no gesture with that ID in the current
     * frame, then gesture() returns an Invalid Gesture object (rather than a null
     * value). Always check object validity in situations where an gesture might be
     * invalid.
     *
     * @returns bool True, if this is a valid Gesture instance; false, otherwise.
     */
    LEAP_EXPORT bool isValid() const;

    /**
     * Compare Gesture object equality.
     *
     * Two Gestures are equal if they represent the same snapshot of the same
     * recognized movement.
     */
    LEAP_EXPORT bool operator==(const Gesture& rhs) const;

    /**
     * Compare Gesture object inequality.
     *
     * Two Gestures are equal only if they represent the same snapshot of the same
     * recognized movement.
     */
    LEAP_EXPORT bool operator!=(const Gesture& rhs) const;

    /**
     * A string containing a brief, human-readable description of this
     * Gesture.
     */
    LEAP_EXPORT std::string toString() const;

    /**
     * Returns an invalid Gesture object.
     *
     * You can use the instance returned by this function in comparisons testing
     * whether a given Gesture instance is valid or invalid. (You can also use the
     * Gesture::isValid() function.)
     *
     * @returns The invalid Gesture instance.
     */
    LEAP_EXPORT static const Gesture& invalid();
};

/**
 * The SwipeGesture class represents a swiping motion of a finger or tool.
 *
 * \image html images/Leap_Gesture_Swipe.png
 *
 * **Important:** To use swipe gestures in your application, you must enable
 * recognition of the swipe gesture. You can enable recognition with:
 *
 * \include Gesture_Swipe_Enable.txt
 *
 * Swipe gestures are continuous.
 *
 * You can set the minimum length and velocity required for a movement
 * to be recognized as a swipe using the config attribute of a connected
 * Controller object. Use the following keys to configure swipe recognition:
 *
 * Key string | Value type | Default value | Units
 * -----------|------------|---------------|------
 * Gesture.Swipe.MinLength | float | 150 | mm
 * Gesture.Swipe.MinVelocity | float | 1000 | mm/s
 *
 * The following example demonstrates how to set the swipe configuration
 * parameters:
 *
 * \include Gesture_Swipe_Params.txt
 */
class SwipeGesture : public Gesture
{
  public:
    /**
     * The swipe gesture type.
     *
     * @returns Type The type value designating a swipe gesture.
     */
    static Type classType() { return TYPE_SWIPE; }

    LEAP_EXPORT SwipeGesture();

    /**
     * Constructs a SwipeGesture object from an instance of the Gesture class.
     *
     * @param rhs The Gesture instance to specialize. This Gesture instance must
     * be a SwipeGesture object.
     */
    LEAP_EXPORT SwipeGesture(const Gesture& rhs);

    /**
     * The position where the swipe began.
     *
     * @returns Vector The starting position within the Leap frame of
     * reference, in mm.
     */
    LEAP_EXPORT Vector startPosition() const;

    /**
     * The current position of the swipe.
     *
     * @returns Vector The current swipe position within the Leap frame of
     * reference, in mm.
     */
    LEAP_EXPORT Vector position() const;

    /**
     * The unit direction vector parallel to the swipe motion.
     *
     * You can compare the components of the vector to classify the swipe as
     * appropriate for your application. For example, if you are using swipes
     * for two dimensional scrolling, you can compare the x and y values to
     * determine if the swipe is primarily horizontal or vertical.
     *
     * @returns Vector The unit direction vector representing the swipe
     * motion.
     */
    LEAP_EXPORT Vector direction() const;

    /**
     * The swipe speed in mm/second.
     *
     * @returns float The speed of the finger performing the swipe gesture in
     * millimeters per second.
     */
    LEAP_EXPORT float speed() const;

    /**
     * The finger performing the swipe gesture.
     *
     * @returns Pointable A Pointable object representing the swiping finger.
     */
    LEAP_EXPORT Pointable pointable() const;
};

/**
 * The CircleGesture classes represents a circular finger movement.
 *
 * A circle movement is recognized when the tip of a finger draws a circle
 * within the Leap field of view.
 *
 * \image html images/Leap_Gesture_Circle.png
 *
 * **Important:** To use circle gestures in your application, you must enable
 * recognition of the circle gesture. You can enable recognition with:
 *
 * \include Gesture_Circle_Enable.txt
 *
 * Circle gestures are continuous. The CircleGesture objects for the gesture have
 * three possible states:
 *
 * * State::STATE_START -- The circle gesture has just started. The movement has
 *  progressed far enough for the recognizer to classify it as a circle.
 * * State::STATE_UPDATE -- The circle gesture is continuing.
 * * State::STATE_STOP -- The circle gesture is finished.
 *
 * You can set the minimum radius and minimum arc length required for a movement
 * to be recognized as a circle using the config attribute of a connected 
 * Controller object. Use the following keys to configure circle recognition:
 *
 * Key string | Value type | Default value | Units
 * -----------|------------|---------------|------
 * Gesture.Circle.MinRadius | float | 5.0 | mm
 * Gesture.Circle.MinArc | float | 1.5*pi | radians
 *
 * The following example demonstrates how to set the circle configuration
 * parameters:
 *
 * \include Gesture_Circle_Params.txt
 */
class CircleGesture : public Gesture
{
  public:
    /**
     * The circle gesture type.
     *
     * @returns Type The type value designating a circle gesture.
     */
    static Type classType() { return TYPE_CIRCLE; }

    /**
     * Constructs a new CircleGesture object.
     *
     * An uninitialized CircleGesture object is considered invalid. Get valid instances
     * of the CircleGesture class from a Frame object.
     */
    LEAP_EXPORT CircleGesture();

    /**
     * Constructs a CircleGesture object from an instance of the Gesture class.
     *
     * @param rhs The Gesture instance to specialize. This Gesture instance must
     * be a CircleGesture object.
     */
    LEAP_EXPORT CircleGesture(const Gesture& rhs);

    /**
     * The center point of the circle within the Leap frame of reference.
     *
     * @returns Vector The center of the circle in mm from the Leap origin.
     */
    LEAP_EXPORT Vector center() const;

    /**
     * Returns the normal vector for the circle being traced.
     *
     * If you draw the circle clockwise, the normal vector points in the same
     * general direction as the pointable object drawing the circle. If you draw
     * the circle counterclockwise, the normal points back toward the
     * pointable. If the angle between the normal and the pointable object
     * drawing the circle is less than 90 degrees, then the circle is clockwise.
     *
     *    std::string clockwiseness;
     *    if (circle.pointable().direction().angleTo(circle.normal()) <= PI/4) {
     *        clockwiseness = "clockwise";
     *    }
     *    else
     *    {
     *        clockwiseness = "counterclockwise";
     *    }
     *
     * @return Vector the normal vector for the circle being traced
     */
    LEAP_EXPORT Vector normal() const;

    /**
     * The number of times the finger tip has traversed the circle.
     *
     * Progress is reported as a positive number of the number. For example,
     * a progress value of .5 indicates that the finger has gone halfway
     * around, while a value of 3 indicates that the finger has gone around
     * the the circle three times.
     *
     * Progress starts where the circle gesture began. Since it the circle
     * must be partially formed before the Leap can recognize it, progress
     * will be greater than zero when a circle gesture first appears in the
     * frame.
     *
     * @returns float A positive number indicating the gesture progress.
     */
    LEAP_EXPORT float progress() const;

    /**
     * The radius of the circle.
     *
     * @returns The circle radius in mm.
     */
    LEAP_EXPORT float radius() const;

    /**
     * The finger performing the circle gesture.
     *
     * @returns Pointable A Pointable object representing the circling finger.
     */
    LEAP_EXPORT Pointable pointable() const;
};

/**
 * The ScreenTapGesture class represents a tapping gesture by a finger or tool.
 *
 * A screen tap gesture is recognized when the tip of a finger pokes forward
 * and then springs back to approximately the original position, as if
 * tapping a vertical screen. The tapping finger must pause briefly before beginning the tap.
 *
 * \image html images/Leap_Gesture_Tap2.png
 *
 * **Important:** To use screen tap gestures in your application, you must enable
 * recognition of the screen tap gesture. You can enable recognition with:
 *
 * \include Gesture_ScreenTap_Enable.txt
 *
 * ScreenTap gestures are discrete. The ScreenTapGesture object representing a tap always
 * has the state, STATE_STOP. Only one ScreenTapGesture object is created for each
 * screen tap gesture recognized.
 *
 * You can set the minimum finger movement and velocity required for a movement
 * to be recognized as a screen tap as well as adjust the detection window for
 * evaluating the movement using the config attribute of a connected
 * Controller object. Use the following keys to configure screen tap recognition:
 *
 * Key string | Value type | Default value | Units
 * -----------|------------|---------------|------
 * Gesture.ScreenTap.MinForwardVelocity  | float | 50 | mm/s
 * Gesture.ScreenTap.HistorySeconds | float | 0.1 | s
 * Gesture.ScreenTap.MinDistance | float | 3.0 | mm
 *
 * The following example demonstrates how to set the screen tap configuration
 * parameters:
 *
 * \include Gesture_ScreenTap_Params.txt
 */
class ScreenTapGesture : public Gesture
{
  public:
    /**
     * The screen tap gesture type.
     *
     * @returns Type The type value designating a screen tap gesture.
     */
    static Type classType() { return TYPE_SCREEN_TAP; }

    /**
     * Constructs a new ScreenTapGesture object.
     *
     * An uninitialized ScreenTapGesture object is considered invalid. Get valid instances
     * of the ScreenTapGesture class from a Frame object.
     */
    LEAP_EXPORT ScreenTapGesture();

    /**
     * Constructs a ScreenTapGesture object from an instance of the Gesture class.
     *
     * @param rhs The Gesture instance to specialize. This Gesture instance must
     * be a ScreenTapGesture object.
     */
    LEAP_EXPORT ScreenTapGesture(const Gesture& rhs);

    /**
     * The position where the screen tap is registered.
     *
     * @return Vector A Vector containing the coordinates of screen tap location.
     */
    LEAP_EXPORT Vector position() const;

    /**
     * The direction of finger tip motion.
     *
     * @returns Vector A unit direction vector.
     */
    LEAP_EXPORT Vector direction() const;

    /**
     * The progress value is always 1.0 for a screen tap gesture.
     *
     * @returns float The value 1.0.
     */
    LEAP_EXPORT float progress() const;

    /**
     * The finger performing the screen tap gesture.
     *
     * @returns Pointable A Pointable object representing the tapping finger.
     */
    LEAP_EXPORT Pointable pointable() const;
};

/**
 * The KeyTapGesture class represents a tapping gesture by a finger or tool.
 *
 * A key tap gesture is recognized when the tip of a finger rotates down toward the
 * palm and then springs back to approximately the original position, as if
 * tapping. The tapping finger must pause briefly before beginning the tap.
 *
 * \image html images/Leap_Gesture_Tap.png
 *
 * **Important:** To use key tap gestures in your application, you must enable
 * recognition of the key tap gesture. You can enable recognition with:
 *
 * \include Gesture_KeyTap_Enable.txt
 *
 * Key tap gestures are discrete. The KeyTapGesture object representing a tap always
 * has the state, STATE_STOP. Only one KeyTapGesture object is created for each
 * key tap gesture recognized.
 *
 * You can set the minimum finger movement and velocity required for a movement
 * to be recognized as a key tap as well as adjust the detection window for
 * evaluating the movement using the config attribute of a connected
 * Controller object. Use the following configuration keys to configure key tap
 * recognition:
 *
 * Key string | Value type | Default value | Units
 * -----------|------------|---------------|------
 * Gesture.KeyTap.MinDownVelocity | float | 50 | mm/s
 * Gesture.KeyTap.HistorySeconds | float | 0.1 | s
 * Gesture.KeyTap.MinDistance | float | 5.0 | mm
 *
 * The following example demonstrates how to set the key tap configuration
 * parameters:
 *
 * \include Gesture_KeyTap_Params.txt
 */
class KeyTapGesture : public Gesture
{
  public:
    /**
     * The key tap gesture type.
     *
     * @returns Type The type value designating a key tap gesture.
     */
    static Type classType() { return TYPE_KEY_TAP; }

    /**
     * Constructs a new KeyTapGesture object.
     *
     * An uninitialized KeyTapGesture object is considered invalid. Get valid instances
     * of the KeyTapGesture class from a Frame object.
     */
    LEAP_EXPORT KeyTapGesture();

    /**
     * Constructs a KeyTapGesture object from an instance of the Gesture class.
     *
     * @param rhs The Gesture instance to specialize. This Gesture instance must
     * be a KeyTapGesture object.
     */
    LEAP_EXPORT KeyTapGesture(const Gesture& rhs);

    /**
     * The position where the key tap is registered.
     *
     * @return Vector A Vector containing the coordinates of tap location.
     */
    LEAP_EXPORT Vector position() const;

    /**
     * The direction of finger tip motion.
     *
     * @returns Vector A unit direction vector if the finger tip is moving;
     * otherwise, a zero-vector.
     */
    LEAP_EXPORT Vector direction() const;

    /**
     * The progress value is always 1.0 for a key tap gesture.
     *
     * @returns float The value 1.0.
     */
    LEAP_EXPORT float progress() const;

    /**
     * The finger performing the key tap gesture.
     *
     * @returns Pointable A Pointable object representing the tapping finger.
     */
    LEAP_EXPORT Pointable pointable() const;
};

/**
 * The Screen class represents a computer monitor screen.
 *
 * The Screen class reports characteristics describing the position and
 * orientation of the monitor screen within the Leap coordinate system. These
 * characteristics include the bottom-left corner position of the screen,
 * direction vectors for the horizontal and vertical axes of the screen, and
 * the screen's normal vector. The screen must be properly registered with the
 * Screen Locator for the Leap to report these characteristics accurately.
 * The Screen class also reports the size of the screen in pixels, using
 * information obtained from the operating system. (Run the Screen Locator
 * from the Leap Application Settings dialog, on the Screen page.)
 *
 * You can get the point of intersection between the screen and a ray
 * projected from a Pointable object using the Screen::intersect() function.
 * Likewise, you can get the closest point on the screen to a point in space
 * using the Screen::project() function. Again, the screen location
 * must be registered with the Screen Locator for these functions to
 * return accurate values.
 *
 * Note that Screen objects can be invalid, which means that they do not contain
 * valid screen coordinate data and do not correspond to a physical entity.
 * Test for validity with the Screen::isValid() function.
 */
class Screen : public Interface {
  public:
    // For internal use only.
    Screen(ScreenImplementation*);

    /**
     * Constructs a Screen object.
     *
     * An uninitialized screen is considered invalid.
     * Get valid Screen objects from a ScreenList object obtained using the
     * Controller::locatedScreens() method.
     */
    LEAP_EXPORT Screen();

    /**
     * A unique identifier for this screen based on the screen
     * information in the configuration. A default screen with ID, *0*,
     * always exists and contains default characteristics, even if no screens
     * have been located.
     */
    LEAP_EXPORT int32_t id() const;

    /**
     * Returns the intersection between this screen and a ray projecting from a
     * Pointable object.
     *
     * The projected ray emanates from the Pointable tipPosition along the
     * Pointable's direction vector.
     *
     * Set the normalize parameter to true to request the intersection point in
     * normalized screen coordinates. Normalized screen coordinates are usually
     * values between 0 and 1, where 0 represents the screen's origin at the
     * bottom-left corner and 1 represents the opposite edge (either top or
     * right). When you request normalized coordinates, the z-component of the
     * returned vector is zero. Multiply a normalized coordinate by the values
     * returned by Screen::widthPixels() or Screen::heightPixels() to calculate
     * the screen position in pixels (remembering that many other computer
     * graphics coordinate systems place the origin in the top-left corner).
     *
     * Set the normalize parameter to false to request the intersection point
     * in Leap coordinates (millimeters from the Leap origin).
     *
     * If the Pointable object points outside the screen's border (but still
     * intersects the plane in which the screen lies), the returned intersection
     * point is clamped to the nearest point on the edge of the screen.
     *
     * You can use the clampRatio parameter to contract or expand the area in
     * which you can point. For example, if you set the clampRatio parameter to
     * 0.5, then the positions reported for intersection points outside the
     * central 50% of the screen are moved to the border of this smaller area.
     * If, on the other hand, you expanded the area by setting clampRatio to
     * a value such as 3.0, then you could point well outside screen's physical
     * boundary before the intersection points would be clamped. The positions
     * for any points clamped would also be placed on this larger outer border.
     * The positions reported for any intersection points inside the clamping
     * border are unaffected by clamping.
     *
     * \include Screen_Normalized_2.txt
     *
     * If the Pointable object does not point toward the plane of the screen
     * (i.e. it is pointing parallel to or away from the screen), then the
     * components of the returned vector are all set to NaN (not-a-number).
     *
     * @param pointable The Pointable object to check for screen intersection.
     *
     * @param normalize If true, return normalized coordinates representing
     * the intersection point as a percentage of the screen's width and height.
     * If false, return Leap coordinates (millimeters from the Leap origin,
     * which is located at the center of the top surface of the Leap device).
     * If true and the clampRatio parameter is set to 1.0, coordinates will be
     * of the form (0..1, 0..1, 0). Setting the clampRatio to a different value
     * changes the range for normalized coordinates. For example, a clampRatio
     * of 5.0 changes the range of values to be of the form (-2..3, -2..3, 0).
     *
     * @param clampRatio Adjusts the clamping border around this screen.
     * By default this ratio is 1.0, and the border corresponds to the actual
     * boundaries of the screen. Setting clampRatio to 0.5 would reduce the
     * interaction area. Likewise, setting the ratio to 2.0 would increase the
     * interaction area, adding 50% around each edge of the physical monitor.
     * Intersection points outside the interaction area are repositioned to
     * the closest point on the clamping border before the vector is returned.
     *
     * @returns A Vector containing the coordinates of the intersection between
     * this screen and a ray projecting from the specified Pointable object.
     */
    LEAP_EXPORT Vector intersect(const Pointable& pointable, bool normalize, float clampRatio = 1.0f) const;

    /**
     * Returns the intersection between this screen and a ray projecting from
     * the specified position along the specified direction.
     *
     * Set the normalize parameter to true to request the intersection point in
     * normalized screen coordinates. Normalized screen coordinates are usually
     * values between 0 and 1, where 0 represents the screen's origin at the
     * bottom-left corner and 1 represents the opposite edge (either top or
     * right). When you request normalized coordinates, the z-component of the
     * returned vector is zero. Multiply a normalized coordinate by the values
     * returned by Screen::widthPixels() or Screen::heightPixels() to calculate
     * the screen position in pixels (remembering that many other computer
     * graphics coordinate systems place the origin in the top-left corner).
     *
     * Set the normalize parameter to false to request the intersection point
     * in Leap coordinates (millimeters from the Leap origin).
     *
     * If the specified ray points outside the screen's border (but still
     * intersects the plane in which the screen lies), the returned intersection
     * point is clamped to the nearest point on the edge of the screen.
     *
     * You can use the clampRatio parameter to contract or expand the area in
     * which you can point. For example, if you set the clampRatio parameter to
     * 0.5, then the positions reported for intersection points outside the
     * central 50% of the screen are moved to the border of this smaller area.
     * If, on the other hand, you expanded the area by setting clampRatio to
     * a value such as 3.0, then you could point well outside screen's physical
     * boundary before the intersection points would be clamped. The positions
     * for any points clamped would also be placed on this larger outer border.
     * The positions reported for any intersection points inside the clamping
     * border are unaffected by clamping.
     *
     * If the specified ray does not point toward the plane of the screen
     * (i.e. it is pointing parallel to or away from the screen), then the
     * components of the returned vector are all set to NaN (not-a-number).
     *
     * @param position The position from which to check for screen intersection.
     * @param direction The direction in which to check for screen intersection.
     *
     * @param normalize If true, return normalized coordinates representing
     * the intersection point as a percentage of the screen's width and height.
     * If false, return Leap coordinates (millimeters from the Leap origin,
     * which is located at the center of the top surface of the Leap device).
     * If true and the clampRatio parameter is set to 1.0, coordinates will be
     * of the form (0..1, 0..1, 0). Setting the clampRatio to a different value
     * changes the range for normalized coordinates. For example, a clampRatio
     * of 5.0 changes the range of values to be of the form (-2..3, -2..3, 0).
     *
     * @param clampRatio Adjusts the clamping border around this screen.
     * By default this ratio is 1.0, and the border corresponds to the actual
     * boundaries of the screen. Setting clampRatio to 0.5 would reduce the
     * interaction area. Likewise, setting the ratio to 2.0 would increase the
     * interaction area, adding 50% around each edge of the physical monitor.
     * Intersection points outside the interaction area are repositioned to
     * the closest point on the clamping border before the vector is returned.
     *
     * @returns A Vector containing the coordinates of the intersection between
     * this screen and a ray projecting from the specified position in the
     * specified direction.
     */
    LEAP_EXPORT Vector intersect(const Vector& position, const Vector& direction, bool normalize, float clampRatio = 1.0f) const;

    /**
     * Returns the projection from the specified position onto this screen.
     *
     * Set the normalize parameter to true to request the projection point in
     * normalized screen coordinates. Normalized screen coordinates are usually
     * values between 0 and 1, where 0 represents the screen's origin at the
     * bottom-left corner and 1 represents the opposite edge (either top or
     * right). When you request normalized coordinates, the z-component of the
     * returned vector is zero. Multiply a normalized coordinate by the values
     * returned by Screen::widthPixels() or Screen::heightPixels() to calculate
     * the screen position in pixels (remembering that many other computer
     * graphics coordinate systems place the origin in the top-left corner).
     *
     * Set the normalize parameter to false to request the projection point
     * in Leap coordinates (millimeters from the Leap origin).
     *
     * If the specified point projects outside the screen's border, the returned
     * projection point is clamped to the nearest point on the edge of the screen.
     *
     * You can use the clampRatio parameter to contract or expand the area in
     * which you can point. For example, if you set the clampRatio parameter to
     * 0.5, then the positions reported for projection points outside the
     * central 50% of the screen are moved to the border of this smaller area.
     * If, on the other hand, you expanded the area by setting clampRatio to
     * a value such as 3.0, then you could point well outside screen's physical
     * boundary before the projection points would be clamped. The positions
     * for any points clamped would also be placed on this larger outer border.
     * The positions reported for any projection points inside the clamping
     * border are unaffected by clamping.
     *
     * @param position The position from which to project onto this screen.
     *
     * @param normalize If true, return normalized coordinates representing
     * the projection point as a percentage of the screen's width and height.
     * If false, return Leap coordinates (millimeters from the Leap origin,
     * which is located at the center of the top surface of the Leap device).
     * If true and the clampRatio parameter is set to 1.0, coordinates will be
     * of the form (0..1, 0..1, 0). Setting the clampRatio to a different value
     * changes the range for normalized coordinates. For example, a clampRatio
     * of 5.0 changes the range of values to be of the form (-2..3, -2..3, 0).
     *
     * @param clampRatio Adjusts the clamping border around this screen.
     * By default this ratio is 1.0, and the border corresponds to the actual
     * boundaries of the screen. Setting clampRatio to 0.5 would reduce the
     * interaction area. Likewise, setting the ratio to 2.0 would increase the
     * interaction area, adding 50% around each edge of the physical monitor.
     * Projection points outside the interaction area are repositioned to
     * the closest point on the clamping border before the vector is returned.
     *
     * @returns A Vector containing the coordinates of the projection between
     * this screen and a ray projecting from the specified position onto the
     * screen along its normal vector.
     */
    LEAP_EXPORT Vector project(const Vector& position, bool normalize, float clampRatio = 1.0f) const;

    /**
     * A Vector representing the horizontal axis of this Screen within the
     * Leap coordinate system.
     *
     * The magnitude of this vector estimates the physical width of this Screen
     * in millimeters. The direction of this vector is parallel to the bottom
     * edge of the screen and points toward the right edge of the screen.
     *
     * Together, horizontalAxis(), verticalAxis(), and bottomLeftCorner()
     * describe the physical position, size and orientation of this Screen.
     *
     * @returns A Vector representing the bottom, horizontal edge of this Screen.
     */
    LEAP_EXPORT Vector horizontalAxis() const;

    /**
     * A Vector representing the vertical axis of this Screen within the
     * Leap coordinate system.
     *
     * The magnitude of this vector estimates the physical height of this Screen
     * in millimeters. The direction of this vector is parallel to the left
     * edge of the screen and points toward the top edge of the screen.
     *
     * Together, horizontalAxis(), verticalAxis(), and bottomLeftCorner()
     * describe the physical position, size and orientation of this screen.
     *
     * @returns A Vector representing the left, vertical edge of this Screen.
     */
    LEAP_EXPORT Vector verticalAxis() const;

    /**
     * A Vector representing the bottom left corner of this Screen within the
     * Leap coordinate system.
     *
     * The point represented by this vector defines the origin of the screen
     * in the Leap coordinate system.
     *
     * Together, horizontalAxis(), verticalAxis(), and bottomLeftCorner()
     * describe the physical position, size and orientation of this Screen.
     *
     * @returns A Vector containing the coordinates of the bottom-left corner
     * of this Screen.
     */
    LEAP_EXPORT Vector bottomLeftCorner() const;

    /**
     * A Vector normal to the plane in which this Screen lies.
     *
     * The normal vector is a unit direction vector orthogonal to the screen's
     * surface plane. It points toward a viewer positioned for typical use of
     * the monitor.
     *
     * @returns A Vector representing this Screen's normal vector.
     */
    LEAP_EXPORT Vector normal() const;

    /**
     * The horizontal resolution of this screen, in pixels.
     *
     * @returns The width of this Screen in pixels.
     */
    LEAP_EXPORT int widthPixels() const;

    /**
     * The vertical resolution of this screen, in pixels.
     *
     * @returns The height of this Screen in pixels.
     */
    LEAP_EXPORT int heightPixels() const;

    /**
     * The shortest distance from the specified point to the plane in which this
     * Screen lies.
     *
     * @returns The length of the perpendicular line segment extending from
     * the plane this Screen lies in to the specified point.
     */
    LEAP_EXPORT float distanceToPoint(const Vector& point) const;

    /**
     * Reports whether this is a valid Screen object.
     *
     * **Important:** A valid Screen object does not necessarily contain
     * up-to-date screen location information. Location information is only
     * accurate until the Leap device or the monitor are moved. In addition, the
     * primary screen always contains default location information even if the
     * user has never run the screen location utility. This default location
     * information will not return accurate results.
     *
     * @returns True, if this Screen object contains valid data.
     */
    LEAP_EXPORT bool isValid() const;

    /**
     * Returns an invalid Screen object.
     *
     * You can use the instance returned by this function in comparisons testing
     * whether a given Screen instance is valid or invalid. (You can also use the
     * Screen::isValid() function.)
     *
     * @returns The invalid Screen instance.
     */
    LEAP_EXPORT static const Screen& invalid();

    /**
     * Compare Screen object equality.
     * Two Screen objects are equal if and only if both Screen objects represent the
     * exact same Screens and both Screens are valid.
     */
    LEAP_EXPORT bool operator==(const Screen&) const;

    /**
     * Compare Screen object inequality.
     * Two Screen objects are equal if and only if both Screen objects represent the
     * exact same Screens and both Screens are valid.
     */
    LEAP_EXPORT bool operator!=(const Screen&) const;

    /** Writes a brief, human readable description of the Screen object. */
    LEAP_EXPORT friend std::ostream& operator<<(std::ostream&, const Screen&);

    /**
     * A string containing a brief, human readable description of the Screen object.
     *
     * @returns A description of the Screen as a string.
     */
    LEAP_EXPORT std::string toString() const;
};

// For internal use only.
template<typename L, typename T>
class ConstListIterator {
  public:
    ConstListIterator<L,T>(const L& list, int index) : m_list(list), m_index(index) {}

    const T operator*() const { return m_list[m_index]; }
    const ConstListIterator<L,T>& operator++() { ++m_index; return *this; }
    bool operator!=(const ConstListIterator<L,T>& rhs) const { return m_index != rhs.m_index; }

    typedef std::ptrdiff_t difference_type;
    typedef T value_type;
    typedef const T* pointer;
    typedef const T& reference;
    typedef std::forward_iterator_tag iterator_category;

  private:
    const L& m_list;
    int m_index;
};

/**
 * The PointableList class represents a list of Pointable objects.
 *
 * Pointable objects include entities that can be pointed, such as fingers and tools.
 *
 * Get a PointableList object by calling Frame::pointables().
 */
class PointableList : public Interface {
  public:
    // For internal use only.
    PointableList(const ListBaseImplementation<Pointable>&);

    /** Constructs an empty list of pointable entities. */
    LEAP_EXPORT PointableList();

    /**
     * Returns the number of pointable entities in this list.
     * @returns The number of pointable entities in this list.
     */
    LEAP_EXPORT int count() const;

    /**
     * Reports whether the list is empty.
     * @returns True, if the list has no members.
     */
    LEAP_EXPORT bool isEmpty() const;

    /**
     * Deprecated. Use PointableList::isEmpty() instead.
     * @see PointableList::isEmpty()
     */
    LEAP_EXPORT bool empty() const;

    /**
     * Access a list member by its position in the list.
     * @param index The zero-based list position index.
     * @returns The Pointable object at the specified index.
     */
    LEAP_EXPORT Pointable operator[](int index) const;

    /**
     * Appends the members of the specified PointableList to this PointableList.
     * @param other A PointableList object containing Pointable objects
     * to append to the end of this PointableList.
     */
    LEAP_EXPORT PointableList& append(const PointableList& other);

    /**
     * Appends the members of the specified FingerList to this PointableList.
     * @param other A FingerList object containing Finger objects
     * to append to the end of this PointableList.
     */
    LEAP_EXPORT PointableList& append(const FingerList& other);

    /**
     * Appends the members of the specified ToolList to this PointableList.
     * @param other A ToolList object containing Tool objects
     * to append to the end of this PointableList.
     */
    LEAP_EXPORT PointableList& append(const ToolList& other);

    /**
     * The member of the list that is farthest to the left within the standard
     * Leap frame of reference (i.e has the smallest X coordinate).
     *
     * @returns The leftmost pointable, or invalid if list is empty. 
     */
    LEAP_EXPORT Pointable leftmost() const;

    /**
     * The member of the list that is farthest to the right within the standard
     * Leap frame of reference (i.e has the largest X coordinate).
     *
     * @returns The rightmost pointable, or invalid if list is empty. 
     */
    LEAP_EXPORT Pointable rightmost() const;


    /**
     * The member of the list that is farthest to the front within the standard
     * Leap frame of reference (i.e has the smallest Z coordinate).
     *
     * @returns The frontmost pointable, or invalid if list is empty. 
     */
    LEAP_EXPORT Pointable frontmost() const;

    /** A C++ iterator type for PointableList objects. */
    typedef ConstListIterator<PointableList, Pointable> const_iterator;

     /** The C++ iterator set to the beginning of this PointableList. */
    LEAP_EXPORT const_iterator begin() const;

    /** The C++ iterator set to the end of this PointableList. */
    LEAP_EXPORT const_iterator end() const;
};

/**
 * The FingerList class represents a list of Finger objects.
 *
 * Get a FingerList object by calling Frame::fingers().
 */
class FingerList : public Interface {
  public:
    // For internal use only.
    FingerList(const ListBaseImplementation<Finger>&);

    /** Constructs an empty list of fingers. */
    LEAP_EXPORT FingerList();

    /**
     * Returns the number of fingers in this list.
     * @returns The number of fingers in this list.
     */
    LEAP_EXPORT int count() const;

    /**
     * Reports whether the list is empty.
     * @returns True, if the list has no members.
     */
    LEAP_EXPORT bool isEmpty() const;

    /**
     * Deprecated. Use FingerList::isEmpty() instead.
     * @see FingerList::isEmpty()
     */
    LEAP_EXPORT bool empty() const;

    /**
     * Access a list member by its position in the list.
     * @param index The zero-based list position index.
     * @returns The Finger object at the specified index.
     */
    LEAP_EXPORT Finger operator[](int index) const;

    /**
     * Appends the members of the specified FingerList to this FingerList.
     * @param other A FingerList object containing Finger objects
     * to append to the end of this FingerList.
     */
    LEAP_EXPORT FingerList& append(const FingerList& other);

    /**
     * The member of the list that is farthest to the left within the standard
     * Leap frame of reference (i.e has the smallest X coordinate).
     *
     * @returns The leftmost finger, or invalid if list is empty. 
     */
    LEAP_EXPORT Finger leftmost() const;

    /**
     * The member of the list that is farthest to the right within the standard
     * Leap frame of reference (i.e has the largest X coordinate).
     *
     * @returns The rightmost finger, or invalid if list is empty. 
     */
    LEAP_EXPORT Finger rightmost() const;

    /**
     * The member of the list that is farthest to the front within the standard
     * Leap frame of reference (i.e has the smallest Z coordinate).
     *
     * @returns The frontmost finger, or invalid if list is empty. 
     */
    LEAP_EXPORT Finger frontmost() const;

    /** A C++ iterator type for FingerList objects. */
    typedef ConstListIterator<FingerList, Finger> const_iterator;

    /** The C++ iterator set to the beginning of this FingerList. */
    LEAP_EXPORT const_iterator begin() const;

    /** The C++ iterator set to the end of this FingerList. */
    LEAP_EXPORT const_iterator end() const;
};

/**
 * The ToolList class represents a list of Tool objects.
 *
 * Get a ToolList object by calling Frame::tools().
 */
class ToolList : public Interface {
  public:
    // For internal use only.
    ToolList(const ListBaseImplementation<Tool>&);

    /** Constructs an empty list of tools. */
    LEAP_EXPORT ToolList();

    /**
     * Returns the number of tools in this list.
     * @returns The number of tools in this list.
     */
    LEAP_EXPORT int count() const;

    /**
     * Reports whether the list is empty.
     * @returns True, if the list has no members.
     */
    LEAP_EXPORT bool isEmpty() const;

    /**
     * Deprecated. Use ToolList::isEmpty() instead.
     * @see ToolList::isEmpty()
     */
    LEAP_EXPORT bool empty() const;

    /**
     * Access a list member by its position in the list.
     * @param index The zero-based list position index.
     * @returns The Tool object at the specified index.
     */
    LEAP_EXPORT Tool operator[](int index) const;

    /**
     * Appends the members of the specified ToolList to this ToolList.
     * @param other A ToolList object containing Tool objects
     * to append to the end of this ToolList.
     */
    LEAP_EXPORT ToolList& append(const ToolList& other);

    /** 
     * The member of the list that is farthest to the left within the standard
     * Leap frame of reference (i.e has the smallest X coordinate).
     *
     * @returns The leftmost tool, or invalid if list is empty. 
     */
    LEAP_EXPORT Tool leftmost() const;

    /** 
     * The member of the list that is farthest to the right within the standard
     * Leap frame of reference (i.e has the largest X coordinate).
     *
     * @returns The rightmost tool, or invalid if list is empty. 
     */
    LEAP_EXPORT Tool rightmost() const;

    /** 
     * The member of the list that is farthest to the front within the standard
     * Leap frame of reference (i.e has the smallest Z coordinate).
     *
     * @returns The frontmost tool, or invalid if list is empty. 
     */
    LEAP_EXPORT Tool frontmost() const;

    /** A C++ iterator type for ToolList objects. */
    typedef ConstListIterator<ToolList, Tool> const_iterator;

    /** The C++ iterator set to the beginning of this ToolList. */
    LEAP_EXPORT const_iterator begin() const;

    /** The C++ iterator set to the end of this ToolList. */
    LEAP_EXPORT const_iterator end() const;
};

/**
 * The HandList class represents a list of Hand objects.
 *
 * Get a HandList object by calling Frame::hands().
 */
class HandList : public Interface {
  public:
    // For internal use only.
    HandList(const ListBaseImplementation<Hand>&);

    /** Constructs an empty list of hands. */
    LEAP_EXPORT HandList();

    /**
     * Returns the number of hands in this list.
     * @returns The number of hands in this list.
     */
    LEAP_EXPORT int count() const;

    /**
     * Reports whether the list is empty.
     * @returns True, if the list has no members.
     */
    LEAP_EXPORT bool isEmpty() const;

    /**
     * Deprecated. Use HandList::isEmpty() instead.
     * @see HandList::isEmpty()
     */
    LEAP_EXPORT bool empty() const;

    /**
     * Access a list member by its position in the list.
     * @param index The zero-based list position index.
     * @returns The Hand object at the specified index.
     */
    LEAP_EXPORT Hand operator[](int index) const;

    /**
     * Appends the members of the specified HandList to this HandList.
     * @param other A HandList object containing Hand objects
     * to append to the end of this HandList.
     */
    LEAP_EXPORT HandList& append(const HandList& other);

    /**
     * The member of the list that is farthest to the left within the standard
     * Leap frame of reference (i.e has the smallest X coordinate).
     *
     * @returns The leftmost hand, or invalid if list is empty. 
     */
    LEAP_EXPORT Hand leftmost() const;

    /**
     * The member of the list that is farthest to the right within the standard
     * Leap frame of reference (i.e has the largest X coordinate).
     *
     * @returns The rightmost hand, or invalid if list is empty. 
     */
    LEAP_EXPORT Hand rightmost() const;

    /**
     * The member of the list that is farthest to the front within the standard
     * Leap frame of reference (i.e has the smallest Z coordinate).
     *
     * @returns The frontmost hand, or invalid if list is empty. 
     */
    LEAP_EXPORT Hand frontmost() const;

    /** A C++ iterator type for this HandList objects. */
    typedef ConstListIterator<HandList, Hand> const_iterator;

    /** The C++ iterator set to the beginning of this HandList. */
    LEAP_EXPORT const_iterator begin() const;

    /** The C++ iterator set to the end of this HandList. */
    LEAP_EXPORT const_iterator end() const;
};

/**
 * The GestureList class represents a list of Gesture objects.
 *
 * Get a GestureList object from a Frame object.
 */
class GestureList : public Interface {
  public:
    // For internal use only.
    GestureList(const ListBaseImplementation<Gesture>&);

    /** Constructs an empty gesture list.*/
    LEAP_EXPORT GestureList();

    /**
     * The length of this list.
     * @returns The number of gestures in this list.
     */
    LEAP_EXPORT int count() const;

    /**
     * Reports whether the list is empty.
     * @returns True, if the list has no members.
     */
    LEAP_EXPORT bool isEmpty() const;

    /**
     * Deprecated. Use GestureList::isEmpty() instead.
     * @see GestureList::isEmpty()
     */
    LEAP_EXPORT bool empty() const;

    /**
     * Access a list member by its position in the list.
     * @param index The zero-based list position index.
     * @returns The Gesture object at the specified index.
     */
    LEAP_EXPORT Gesture operator[](int index) const;

    /**
     * Appends the members of the specified GestureList to this GestureList.
     * @param other A GestureList object containing Gesture objects
     * to append to the end of this GestureList.
     */
    LEAP_EXPORT GestureList& append(const GestureList& other);

    /** A C++ iterator type for GestureList objects. */
    typedef ConstListIterator<GestureList, Gesture> const_iterator;

    /** The C++ iterator set to the beginning of this GestureList. */
    LEAP_EXPORT const_iterator begin() const;

    /** The C++ iterator set to the end of this GestureList. */
    LEAP_EXPORT const_iterator end() const;
};

/**
 * The ScreenList class represents a list of Screen objects.
 *
 * The list always contains at least one entry representing the default
 * screen. If the user has not registered the location of this default
 * screen, then the coordinates, directions, and other values reported by
 * the functions in its Screen object will not be accurate. Other monitor
 * screens only appear in the list if their positions have been registered
 * using the Leap Screen Locator.
 *
 * Get a ScreenList object by calling Controller::locatedScreens().
 *
 * \include Screen_Closest_1.txt
 */
class ScreenList : public Interface {
  public:
    // For internal use only.
    ScreenList(const ListBaseImplementation<Screen>&);

    /** Constructs an empty list of screens. */
    LEAP_EXPORT ScreenList();

    /**
     * Returns the number of screens in this list.
     * @returns The number of screens in this list.
     */
    LEAP_EXPORT int count() const;

    /**
     * Reports whether the list is empty.
     * @returns True, if the list has no members.
     */
    LEAP_EXPORT bool isEmpty() const;

    /**
     * Deprecated. Use ScreenList::isEmpty() instead.
     * @see ScreenList::isEmpty()
     */
    LEAP_EXPORT bool empty() const;

    /**
     * Access a list member by its position in the list.
     * @param index The zero-based list position index.
     * @returns The Screen object at the specified index.
     */
    LEAP_EXPORT Screen operator[](int index) const;

    /** A C++ iterator type for this ScreenList objects. */
    typedef ConstListIterator<ScreenList, Screen> const_iterator;

    /** The C++ iterator set to the beginning of this ScreenList. */
    LEAP_EXPORT const_iterator begin() const;

    /** The C++ iterator set to the end of this ScreenList. */
    LEAP_EXPORT const_iterator end() const;

    /**
     * Gets the closest Screen intercepting a ray projecting from the specified
     * Pointable object.
     *
     * The projected ray emanates from the Pointable tipPosition along the
     * Pointable's direction vector. If the projected ray does not intersect
     * any screen surface directly, then the Leap checks for intersection with
     * the planes extending from the surfaces of the known screens
     * and returns the Screen with the closest intersection.
     *
     * \include Screen_Closest_2.txt
     *
     * If no intersections are found (i.e. the ray is directed parallel to or
     * away from all known screens), then an invalid Screen object is returned.
     *
     * *Note:* Be sure to test whether the Screen object returned by this method
     * is valid. Attempting to use an invalid Screen object will lead to
     * incorrect results.
     *
     * @param pointable The Pointable object to check for screen intersection.
     * @returns The closest Screen toward which the specified Pointable object
     * is pointing, or, if the pointable is not pointing in the direction of
     * any known screen, an invalid Screen object.
     */
    LEAP_EXPORT Screen closestScreenHit(const Pointable& pointable) const;

    /**
     * Gets the closest Screen intercepting a ray projecting from the specified
     * position in the specified direction.
     *
     * The projected ray emanates from the position along the direction vector.
     * If the projected ray does not intersect any screen surface directly,
     * then the Leap checks for intersection with the planes extending from the
     * surfaces of the known screens and returns the Screen with the closest
     * intersection.
     *
     * \include Screen_Closest_3.txt
     *
     * If no intersections are found (i.e. the ray is directed parallel to or
     * away from all known screens), then an invalid Screen object is returned.
     *
     * *Note:* Be sure to test whether the Screen object returned by this method
     * is valid. Attempting to use an invalid Screen object will lead to
     * incorrect results.
     *
     * @param position The position from which to check for screen intersection.
     * @param direction The direction in which to check for screen intersection.
     * @returns The closest Screen toward which the specified ray is pointing,
     * or, if the ray is not pointing in the direction of any known screen,
     * an invalid Screen object.
     */
    LEAP_EXPORT Screen closestScreenHit(const Vector& position, const Vector& direction) const;

    /**
     * Gets the Screen closest to the specified position.
     *
     * The specified position is projected along each screen's normal vector
     * onto the screen's plane. The screen whose projected point is closest to
     * the specified position is returned. Call Screen::project(position)
     * on the returned Screen object to find the projected point.
     *
     * \include Screen_Closest_3.txt
     *
     * @param position The position from which to check for screen projection.
     * @returns The closest Screen onto which the specified position is projected.
     */
    LEAP_EXPORT Screen closestScreen(const Vector& position) const;
};

/**
 * The Frame class represents a set of hand and finger tracking data detected
 * in a single frame.
 *
 * The Leap detects hands, fingers and tools within the tracking area, reporting
 * their positions, orientations and motions in frames at the Leap frame rate.
 *
 * Access Frame objects through an instance of a Leap Controller. Implement a
 * Listener subclass to receive a callback event when a new Frame is available.
 */
class Frame : public Interface {
  public:
    // For internal use only.
    Frame(FrameImplementation*);

    /**
     * Constructs a Frame object.
     *
     * Frame instances created with this constructor are invalid.
     * Get valid Frame objects by calling the Controller::frame() function.
     */
    LEAP_EXPORT Frame();

    /**
     * A unique ID for this Frame. Consecutive frames processed by the Leap
     * have consecutive increasing values.
     *
     * @returns The frame ID.
     */
    LEAP_EXPORT int64_t id() const;

    /**
     * The frame capture time in microseconds elapsed since the Leap started.
     *
     * @returns The timestamp in microseconds.
     */
    LEAP_EXPORT int64_t timestamp() const;

    /**
     * The list of Hand objects detected in this frame, given in arbitrary order.
     * The list can be empty if no hands are detected.
     *
     * @returns The HandList containing all Hand objects detected in this frame.
     */
    LEAP_EXPORT HandList hands() const;

    /**
     * The Hand object with the specified ID in this frame.
     *
     * Use the Frame::hand() function to retrieve the Hand object from
     * this frame using an ID value obtained from a previous frame.
     * This function always returns a Hand object, but if no hand
     * with the specified ID is present, an invalid Hand object is returned.
     *
     * Note that ID values persist across frames, but only until tracking of a
     * particular object is lost. If tracking of a hand is lost and subsequently
     * regained, the new Hand object representing that physical hand may have
     * a different ID than that representing the physical hand in an earlier frame.
     *
     * @param id The ID value of a Hand object from a previous frame.
     * @returns The Hand object with the matching ID if one exists in this frame;
     * otherwise, an invalid Hand object is returned.
     */
    LEAP_EXPORT Hand hand(int32_t id) const;

    /**
     * The list of Pointable objects (fingers and tools) detected in this frame,
     * given in arbitrary order. The list can be empty if no fingers or tools are detected.
     *
     * @returns The PointableList containing all Pointable objects detected in this frame.
     */
    LEAP_EXPORT PointableList pointables() const;

    /**
     * The Pointable object with the specified ID in this frame.
     *
     * Use the Frame::pointable() function to retrieve the Pointable object from
     * this frame using an ID value obtained from a previous frame.
     * This function always returns a Pointable object, but if no finger or tool
     * with the specified ID is present, an invalid Pointable object is returned.
     *
     * Note that ID values persist across frames, but only until tracking of a
     * particular object is lost. If tracking of a finger or tool is lost and subsequently
     * regained, the new Pointable object representing that finger or tool may have
     * a different ID than that representing the finger or tool in an earlier frame.
     *
     * @param id The ID value of a Pointable object from a previous frame.
     * @returns The Pointable object with the matching ID if one exists in this frame;
     * otherwise, an invalid Pointable object is returned.
     */
    LEAP_EXPORT Pointable pointable(int32_t id) const;

    /**
     * The list of Finger objects detected in this frame, given in arbitrary order.
     * The list can be empty if no fingers are detected.
     *
     * @returns The FingerList containing all Finger objects detected in this frame.
     */
    LEAP_EXPORT FingerList fingers() const;

    /**
     * The Finger object with the specified ID in this frame.
     *
     * Use the Frame::finger() function to retrieve the Finger object from
     * this frame using an ID value obtained from a previous frame.
     * This function always returns a Finger object, but if no finger
     * with the specified ID is present, an invalid Finger object is returned.
     *
     * Note that ID values persist across frames, but only until tracking of a
     * particular object is lost. If tracking of a finger is lost and subsequently
     * regained, the new Finger object representing that physical finger may have
     * a different ID than that representing the finger in an earlier frame.
     *
     * @param id The ID value of a Finger object from a previous frame.
     * @returns The Finger object with the matching ID if one exists in this frame;
     * otherwise, an invalid Finger object is returned.
     */
    LEAP_EXPORT Finger finger(int32_t id) const;

    /**
     * The list of Tool objects detected in this frame, given in arbitrary order.
     * The list can be empty if no tools are detected.
     *
     * @returns The ToolList containing all Tool objects detected in this frame.
     */
    LEAP_EXPORT ToolList tools() const;

    /**
     * The Tool object with the specified ID in this frame.
     *
     * Use the Frame::tool() function to retrieve the Tool object from
     * this frame using an ID value obtained from a previous frame.
     * This function always returns a Tool object, but if no tool
     * with the specified ID is present, an invalid Tool object is returned.
     *
     * Note that ID values persist across frames, but only until tracking of a
     * particular object is lost. If tracking of a tool is lost and subsequently
     * regained, the new Tool object representing that tool may have a
     * different ID than that representing the tool in an earlier frame.
     *
     * @param id The ID value of a Tool object from a previous frame.
     * @returns The Tool object with the matching ID if one exists in this frame;
     * otherwise, an invalid Tool object is returned.
     */
    LEAP_EXPORT Tool tool(int32_t id) const;

    /**
     * The Gesture object with the specified ID in this frame.
     *
     * Use the Frame::gesture() function to return a Gesture object in this
     * frame using an ID obtained in an earlier frame. The function always
     * returns a Gesture object, but if there was no update for the gesture in
     * this frame, then an invalid Gesture object is returned.
     *
     * All Gesture objects representing the same recognized movement share the
     * same ID.
     * @param id The ID of an Gesture object from a previous frame.
     * @returns The Gesture object in the frame with the specified ID if one
     * exists; Otherwise, an Invalid Gesture object.
     */
    LEAP_EXPORT Gesture gesture(int32_t id) const;

    /**
     * The gestures recognized or continuing in this frame.
     *
     * Circle and swipe gestures are updated every frame. Tap gestures
     * only appear in the list  for a single frame.
     *
     * @return GestureList the list of gestures.
     */
    LEAP_EXPORT GestureList gestures() const;

    /**
     * Returns a GestureList containing all gestures that have occurred since
     * the specified frame.
     *
     * @param sinceFrame An earlier Frame object. The starting frame must
     * still be in the frame history cache, which has a default length of
     * 60 frames.
     * @return GestureList The list of the Gesture objects that have occurred
     * since the specified frame.
     */
    LEAP_EXPORT GestureList gestures(const Frame& sinceFrame) const;

    /**
     * The change of position derived from the overall linear motion between
     * the current frame and the specified frame.
     *
     * The returned translation vector provides the magnitude and direction of
     * the movement in millimeters.
     *
     * The Leap derives frame translation from the linear motion of
     * all objects detected in the field of view.
     *
     * If either this frame or sinceFrame is an invalid Frame object, then this
     * method returns a zero vector.
     *
     * @param sinceFrame The starting frame for computing the relative translation.
     * @returns A Vector representing the heuristically determined change in
     * position of all objects between the current frame and that specified
     * in the sinceFrame parameter.
     */
    LEAP_EXPORT Vector translation(const Frame& sinceFrame) const;

    /**
     * The estimated probability that the overall motion between the current
     * frame and the specified frame is intended to be a translating motion.
     *
     * If either this frame or sinceFrame is an invalid Frame object, then this
     * method returns zero.
     *
     * @param sinceFrame The starting frame for computing the translation.
     * @returns A value between 0 and 1 representing the estimated probability
     * that the overall motion between the current frame and the specified frame
     * is intended to be a translating motion.
     */
    LEAP_EXPORT float translationProbability(const Frame& sinceFrame) const;

    /**
     * The axis of rotation derived from the overall rotational motion between
     * the current frame and the specified frame.
     *
     * The returned direction vector is normalized.
     *
     * The Leap derives frame rotation from the relative change in position and
     * orientation of all objects detected in the field of view.
     *
     * If either this frame or sinceFrame is an invalid Frame object, or if no
     * rotation is detected between the two frames, a zero vector is returned.
     *
     * @param sinceFrame The starting frame for computing the relative rotation.
     * @returns A normalized direction Vector representing the axis of the
     * heuristically determined rotational change between the current frame
     * and that specified in the sinceFrame parameter.
     */
    LEAP_EXPORT Vector rotationAxis(const Frame& sinceFrame) const;

    /**
     * The angle of rotation around the rotation axis derived from the overall
     * rotational motion between the current frame and the specified frame.
     *
     * The returned angle is expressed in radians measured clockwise around the
     * rotation axis (using the right-hand rule) between the start and end frames.
     * The value is always between 0 and pi radians (0 and 180 degrees).
     *
     * The Leap derives frame rotation from the relative change in position and
     * orientation of all objects detected in the field of view.
     *
     * If either this frame or sinceFrame is an invalid Frame object, then the
     * angle of rotation is zero.
     *
     * @param sinceFrame The starting frame for computing the relative rotation.
     * @returns A positive value containing the heuristically determined
     * rotational change between the current frame and that specified in the
     * sinceFrame parameter.
     */
    LEAP_EXPORT float rotationAngle(const Frame& sinceFrame) const;

    /**
     * The angle of rotation around the specified axis derived from the overall
     * rotational motion between the current frame and the specified frame.
     *
     * The returned angle is expressed in radians measured clockwise around the
     * rotation axis (using the right-hand rule) between the start and end frames.
     * The value is always between -pi and pi radians (-180 and 180 degrees).
     *
     * The Leap derives frame rotation from the relative change in position and
     * orientation of all objects detected in the field of view.
     *
     * If either this frame or sinceFrame is an invalid Frame object, then the
     * angle of rotation is zero.
     *
     * @param sinceFrame The starting frame for computing the relative rotation.
     * @param axis The axis to measure rotation around.
     * @returns A value containing the heuristically determined rotational
     * change between the current frame and that specified in the sinceFrame
     * parameter around the given axis.
     */
    LEAP_EXPORT float rotationAngle(const Frame& sinceFrame, const Vector& axis) const;

    /**
     * The transform matrix expressing the rotation derived from the overall
     * rotational motion between the current frame and the specified frame.
     *
     * The Leap derives frame rotation from the relative change in position and
     * orientation of all objects detected in the field of view.
     *
     * If either this frame or sinceFrame is an invalid Frame object, then this
     * method returns an identity matrix.
     *
     * @param sinceFrame The starting frame for computing the relative rotation.
     * @returns A transformation Matrix containing the heuristically determined
     * rotational change between the current frame and that specified in the
     * sinceFrame parameter.
     */
    LEAP_EXPORT Matrix rotationMatrix(const Frame& sinceFrame) const;

    /**
     * The estimated probability that the overall motion between the current
     * frame and the specified frame is intended to be a rotating motion.
     *
     * If either this frame or sinceFrame is an invalid Frame object, then this
     * method returns zero.
     *
     * @param sinceFrame The starting frame for computing the relative rotation.
     * @returns A value between 0 and 1 representing the estimated probability
     * that the overall motion between the current frame and the specified frame
     * is intended to be a rotating motion.
     */
    LEAP_EXPORT float rotationProbability(const Frame& sinceFrame) const;

    /**
     * The scale factor derived from the overall motion between the current frame
     * and the specified frame.
     *
     * The scale factor is always positive. A value of 1.0 indicates no
     * scaling took place. Values between 0.0 and 1.0 indicate contraction
     * and values greater than 1.0 indicate expansion.
     *
     * The Leap derives scaling from the relative inward or outward motion of
     * all objects detected in the field of view (independent of translation
     * and rotation).
     *
     * If either this frame or sinceFrame is an invalid Frame object, then this
     * method returns 1.0.
     *
     * @param sinceFrame The starting frame for computing the relative scaling.
     * @returns A positive value representing the heuristically determined
     * scaling change ratio between the current frame and that specified in the
     * sinceFrame parameter.
     */
    LEAP_EXPORT float scaleFactor(const Frame& sinceFrame) const;

    /**
     * The estimated probability that the overall motion between the current
     * frame and the specified frame is intended to be a scaling motion.
     *
     * If either this frame or sinceFrame is an invalid Frame object, then this
     * method returns zero.
     *
     * @param sinceFrame The starting frame for computing the relative scaling.
     * @returns A value between 0 and 1 representing the estimated probability
     * that the overall motion between the current frame and the specified frame
     * is intended to be a scaling motion.
     */
    LEAP_EXPORT float scaleProbability(const Frame& sinceFrame) const;

    /**
     * Reports whether this Frame instance is valid.
     *
     * A valid Frame is one generated by the Leap::Controller object that contains
     * tracking data for all detected entities. An invalid Frame contains no
     * actual tracking data, but you can call its functions without risk of a
     * null pointer exception. The invalid Frame mechanism makes it more
     * convenient to track individual data across the frame history. For example,
     * you can invoke:
     *
     * \code{.cpp}
     * Finger finger = controller.frame(n).finger(fingerID);
     * \endcode
     *
     * for an arbitrary Frame history value, "n", without first checking whether
     * frame(n) returned a null object. (You should still check that the
     * returned Finger instance is valid.)
     *
     * @returns True, if this is a valid Frame object; false otherwise.
     */
    LEAP_EXPORT bool isValid() const;

    /**
     * Returns an invalid Frame object.
     *
     * You can use the instance returned by this function in comparisons testing
     * whether a given Frame instance is valid or invalid. (You can also use the
     * Frame::isValid() function.)
     *
     * @returns The invalid Frame instance.
     */
    LEAP_EXPORT static const Frame& invalid();

    /**
     * Compare Frame object equality.
     * Two Frame objects are equal if and only if both Frame objects represent
     * the exact same frame of tracking data and both Frame objects are valid.
     */
    LEAP_EXPORT bool operator==(const Frame&) const;

    /**
     * Compare Frame object inequality.
     * Two Frame objects are equal if and only if both Frame objects represent
     * the exact same frame of tracking data and both Frame objects are valid.
     */
    LEAP_EXPORT bool operator!=(const Frame&) const;

    /** Writes a brief, human readable description of the Frame object to an output stream. */
    LEAP_EXPORT friend std::ostream& operator<<(std::ostream&, const Frame&);

    /**
     * A string containing a brief, human readable description of the Frame object.
     *
     * @returns A description of the Frame as a string.
     */
    LEAP_EXPORT std::string toString() const;
};

/**
 * The Config class provides access to Leap system configuration information.
 *
 * You can get and set gesture configuration parameters using the Config object
 * obtained from a connected Controller object. The key strings required to
 * identify a configuration parameter include:
 *
 * Key string | Value type | Default value | Units
 * -----------|------------|---------------|------
 * Gesture.Circle.MinRadius | float | 5.0 | mm
 * Gesture.Circle.MinArc | float | 1.5*pi | radians
 * Gesture.Swipe.MinLength | float | 150 | mm
 * Gesture.Swipe.MinVelocity | float | 1000 | mm/s
 * Gesture.KeyTap.MinDownVelocity | float | 50 | mm/s
 * Gesture.KeyTap.HistorySeconds | float | 0.1 | s
 * Gesture.KeyTap.MinDistance | float | 3.0 | mm
 * Gesture.ScreenTap.MinForwardVelocity  | float | 50 | mm/s
 * Gesture.ScreenTap.HistorySeconds | float | 0.1 | s
 * Gesture.ScreenTap.MinDistance | float | 5.0 | mm
 *
 * After setting a configuration value, you must call the Config::save method
 * to commit the changes. The configuration value changes are not persistent;
 * your application needs to set the values every time it runs.
 *
 * @see CircleGesture
 * @see KeyTapGesture
 * @see ScreenTapGesture
 * @see SwipeGesture
 */
class Config : public Interface {
  public:
    /** Constructs a Config object. */
    LEAP_EXPORT Config();

    /**
     * Enumerates the possible data types for configuration values.
     *
     * The Config::type() function returns an item from the ValueType enumeration.
     */
    enum ValueType {
      TYPE_UNKNOWN = 0,   /**< The data type is unknown. */
      TYPE_BOOLEAN = 1,   /**< A boolean value. */
      TYPE_INT32   = 2,   /**< A 32-bit integer. */
      TYPE_FLOAT   = 6,   /**< A floating-point number. */
      TYPE_STRING  = 8    /**< A string of characters. */
    };

    /**
     * Reports the natural data type for the value related to the specified key.
     *
     * @param key The key for the looking up the value in the configuration dictionary.
     * @returns The native data type of the value, that is, the type that does not
     * require a data conversion.
     */
    LEAP_EXPORT ValueType type(const std::string& key) const;

    /** Gets the boolean representation for the specified key. */
    LEAP_EXPORT bool getBool(const std::string& key) const;

    /** Sets the boolean representation for the specified key.
     * @returns true on success, false on failure.
     */
    LEAP_EXPORT bool setBool(const std::string& key, bool value);

    /** Gets the 32-bit integer representation for the specified key. */
    LEAP_EXPORT int32_t getInt32(const std::string& key) const;

    /** Sets the 32-bit integer representation for the specified key.
    * @returns true on success, false on failure.
    */
    LEAP_EXPORT bool setInt32(const std::string& key, int32_t value);

    /** Gets the floating point representation for the specified key. */
    LEAP_EXPORT float getFloat(const std::string& key) const;

    /** Sets the floating point representation for the specified key.
    * @returns true on success, false on failure.
    */
    LEAP_EXPORT bool setFloat(const std::string& key, float value);

    /** Gets the string representation for the specified key. */
    LEAP_EXPORT std::string getString(const std::string& key) const;

    /** Sets the string representation for the specified key.
    * @returns true on success, false on failure.
    */
    LEAP_EXPORT bool setString(const std::string& key, const std::string& value);

    /** 
     * Saves the current state of the config.
     *
     * Call ``save()`` after making a set of configuration changes. The
     * ``save()`` function transfers the configuration changes to the Leap
     * application. The configuration value changes are not persistent; your
     * application needs to set the values every time it runs.
     *
     * @returns true on success, false on failure.
     */
    LEAP_EXPORT bool save();
};

/**
 * The Controller class is your main interface to the Leap Motion Controller.
 *
 * Create an instance of this Controller class to access frames of tracking
 * data and configuration information. Frame data can be polled at any time
 * using the Controller::frame() function. Call frame() or frame(0) to get the
 * most recent frame. Set the history parameter to a positive integer to access
 * previous frames. A controller stores up to 60 frames in its frame history.
 *
 * Polling is an appropriate strategy for applications which already have an
 * intrinsic update loop, such as a game. You can also add an instance of a
 * subclass of Leap::Listener to the controller to handle events as they occur.
 * The Leap dispatches events to the listener upon initialization and exiting,
 * on connection changes, and when a new frame of tracking data is available.
 * When these events occur, the controller object invokes the appropriate
 * callback function defined in your subclass of Listener.
 *
 * To access frames of tracking data as they become available:
 *
 * 1. Implement a subclass of the Listener class and override the
 *    Listener::onFrame() function.
 * 2. In your Listener::onFrame() function, call the Controller::frame()
 *    function to access the newest frame of tracking data.
 * 3. To start receiving frames, create a Controller object and add an instance
 *    of the Listener subclass to the Controller::addListener() function.
 *
 * When an instance of a Listener subclass is added to a Controller object,
 * it calls the Listener::onInit() function when the listener is ready for use.
 * When a connection is established between the controller and the Leap, the
 * controller calls the Listener::onConnect() function. At this point, your
 * application will start receiving frames of data. The controller calls the
 * Listener::onFrame() function each time a new frame is available. If the
 * controller loses its connection with the Leap software or device for any
 * reason, it calls the Listener::onDisconnect() function. If the listener is
 * removed from the controller or the controller is destroyed, it calls the
 * Listener::onExit() function. At that point, unless the listener is added to
 * another controller again, it will no longer receive frames of tracking data.
 *
 * The Controller object is multithreaded and calls the Listener functions on
 * its own thread, not on an application thread.
 */
class Controller : public Interface {
  public:
    // For internal use only.
    Controller(ControllerImplementation*);

    /**
     * Constructs a Controller object.
     *
     * When creating a Controller object, you may optionally pass in a
     * reference to an instance of a subclass of Leap::Listener. Alternatively,
     * you may add a listener using the Controller::addListener() function.
     */
    LEAP_EXPORT Controller();
    LEAP_EXPORT virtual ~Controller();
    /**
     * Constructs a Controller object.
     *
     * When creating a Controller object, you may optionally pass in a
     * reference to an instance of a subclass of Leap::Listener. Alternatively,
     * you may add a listener using the Controller::addListener() function.
     *
     * @param listener An instance of Leap::Listener implementing the callback
     * functions for the Leap events you want to handle in your application.
     */
    LEAP_EXPORT Controller(Listener& listener);

    /**
     * Reports whether this Controller is connected to the Leap device.
     *
     * When you first create a Controller object, isConnected() returns false.
     * After the controller finishes initializing and connects to the Leap,
     * isConnected() will return true.
     *
     * You can either handle the onConnect event using a Listener instance or
     * poll the isConnected() function if you need to wait for your
     * application to be connected to the Leap before performing some other
     * operation.
     *
     * @returns True, if connected; false otherwise.
     */
    LEAP_EXPORT bool isConnected() const;

    /**
     * Reports whether this application is the focused, foreground application.
     *
     * Only the foreground application receives tracking information from
     * the Leap Motion Controller.
     *
     * @returns True, if application has focus; false otherwise.
     */

    LEAP_EXPORT bool hasFocus() const;

    /**
     * Adds a listener to this Controller.
     *
     * The Controller dispatches Leap events to each associated listener. The
     * order in which listener callback functions are invoked is arbitrary. If
     * you pass a listener to the Controller's constructor function, it is
     * automatically added to the list and can be removed with the
     * Controller::removeListener() function.
     *
     * @param listener A subclass of Leap::Listener implementing the callback
     * functions for the Leap events you want to handle in your application.
     * @returns Whether or not the listener was successfully added to the list
     * of listeners.
     */
    LEAP_EXPORT bool addListener(Listener& listener);

    /**
     * Remove a listener from the list of listeners that will receive Leap
     * events. A listener must be removed if its lifetime is shorter than the
     * controller to which it is listening.
     *
     * @param listener The listener to remove.
     * @returns Whether or not the listener was successfully removed from the
     * list of listeners.
     */
    LEAP_EXPORT bool removeListener(Listener& listener);

    /**
     * Returns a frame of tracking data from the Leap. Use the optional
     * history parameter to specify which frame to retrieve. Call frame() or
     * frame(0) to access the most recent frame; call frame(1) to access the
     * previous frame, and so on. If you use a history value greater than the
     * number of stored frames, then the controller returns an invalid frame.
     *
     * @param history The age of the frame to return, counting backwards from
     * the most recent frame (0) into the past and up to the maximum age (59).
     * @returns The specified frame; or, if no history parameter is specified,
     * the newest frame. If a frame is not available at the specified history
     * position, an invalid Frame is returned.
     */
    LEAP_EXPORT Frame frame(int history = 0) const;

    /**
     * Returns a Config object, which you can use to query the Leap system for
     * configuration information.
     */
    LEAP_EXPORT Config config() const;

    /**
     * The list of screens whose positions have been identified by using the
     * Leap application Screen Locator.
     *
     * The list always contains at least one entry representing the default
     * screen. If the user has not registered the location of this default
     * screen, then the coordinates, directions, and other values reported by
     * the functions in its Screen object will not be accurate. Other monitor
     * screens only appear in the list if their positions have been registered
     * using the Leap Screen Locator.
     *
     * A Screen object represents the position and orientation of a display
     * monitor screen within the Leap coordinate system.
     * For example, if the screen location is known, you can get Leap coordinates
     * for the bottom-left corner of the screen. Registering the screen
     * location also allows the Leap to calculate the point on the screen at
     * which a finger or tool is pointing.
     *
     * A user can run the Screen Locator tool from the Leap application
     * Settings window. Avoid assuming that a screen location is known or that
     * an existing position is still correct. The registered position is only
     * valid as long as the relative position of the Leap device and the
     * monitor screen remain constant.
     *
     * \include Screen_Closest_1.txt
     *
     * @returns ScreenList A list containing the screens whose positions have
     * been registered by the user using the Screen Locator tool.
     * The list always contains at least one entry representing the default
     * monitor. If the user has not run the Screen Locator or has moved the Leap
     * device or screen since running it, the Screen object for this entry
     * only contains default values.
     */
    LEAP_EXPORT ScreenList locatedScreens() const;

    /**
     * Deprecated. Use Controller::locatedScreens() instead.
     * @see Controller::locatedScreens()
     */
    LEAP_EXPORT ScreenList calibratedScreens() const;

    /**
     * Enables or disables reporting of a specified gesture type.
     *
     * By default, all gesture types are disabled. When disabled, gestures of the
     * disabled type are never reported and will not appear in the frame
     * gesture list.
     *
     * As a performance optimization, only enable recognition for the types
     * of movements that you use in your application.
     *
     * @param type The type of gesture to enable or disable. Must be a
     * member of the Gesture::Type enumeration.
     * @param enable True, to enable the specified gesture type; False,
     * to disable.
     * @see Controller::isGestureEnabled()
     */
    LEAP_EXPORT void enableGesture(Gesture::Type type, bool enable = true) const;

    /**
     * Reports whether the specified gesture type is enabled.
     *
     * @return True, if the specified type is enabled; false, otherwise.
     * @see Controller::enableGesture()
     */
    LEAP_EXPORT bool isGestureEnabled(Gesture::Type type) const;
};

/**
 * The Listener class defines a set of callback functions that you can
 * override in a subclass to respond to events dispatched by the Leap.
 *
 * To handle Leap events, create an instance of a Listener subclass and assign
 * it to the Controller instance. The Controller calls the relevant Listener
 * callback function when an event occurs, passing in a reference to itself.
 * You do not have to implement callbacks for events you do not want to handle.
 *
 * The Controller object calls these Listener functions from a thread created
 * by the Leap library, not the thread used to create or set the Listener instance.
 */
class Listener {
  public:
    /** Constructs a Listener object. */
    LEAP_EXPORT Listener() {}

    /** Destructs this Listener object. */
    LEAP_EXPORT virtual ~Listener() {}

    /**
     * Called once, when this Listener object is newly added to a Controller.
     *
     *  \include Listener_onInit.txt
     *
     * @param controller The Controller object invoking this callback function.
     */
    LEAP_EXPORT virtual void onInit(const Controller&) {}

    /**
     * Called when the Controller object connects to the Leap software, or when
     * this Listener object is added to a Controller that is already connected.
     *
     *  \include Listener_onConnect.txt
     *
     * @param controller The Controller object invoking this callback function.
     */
    LEAP_EXPORT virtual void onConnect(const Controller&) {}

    /**
     * Called when the Controller object disconnects from the Leap software.
     * The controller can disconnect when the Leap device is unplugged, the
     * user shuts the Leap software down, or the Leap software encounters an
     * unrecoverable error.
     *
     *  \include Listener_onDisconnect.txt
     *
     * Note: When you launch a Leap-enabled application in a debugger, the
     * Leap library does not disconnect from the application. This is to allow
     * you to step through code without losing the connection because of time outs.
     *
     * @param controller The Controller object invoking this callback function.
     */
    LEAP_EXPORT virtual void onDisconnect(const Controller&) {}

    /**
     * Called when this Listener object is removed from the Controller
     * or the Controller instance is destroyed.
     *
     *  \include Listener_onExit.txt
     *
     * @param controller The Controller object invoking this callback function.
     */
    LEAP_EXPORT virtual void onExit(const Controller&) {}

    /**
     * Called when a new frame of hand and finger tracking data is available.
     * Access the new frame data using the Controller::frame() function.
     *
     *  \include Listener_onFrame.txt
     *
     * Note, the Controller skips any pending onFrame events while your
     * onFrame handler executes. If your implementation takes too long to return,
     * one or more frames can be skipped. The Controller still inserts the skipped
     * frames into the frame history. You can access recent frames by setting
     * the history parameter when calling the Controller::frame() function.
     * You can determine if any pending onFrame events were skipped by comparing
     * the ID of the most recent frame with the ID of the last received frame.
     *
     * @param controller The Controller object invoking this callback function.
     */
    LEAP_EXPORT virtual void onFrame(const Controller&) {}

    /**
     * Called when this application becomes the foreground application.
     *
     * Only the foreground application receives tracking data from the Leap
     * Motion Controller. This function is only called when the controller
     * object is in a connected state.
     *
     *  \include Listener_onFocusGained.txt
     *
     * @param controller The Controller object invoking this callback function.
     */
    LEAP_EXPORT virtual void onFocusGained(const Controller&) {}

    /**
     * Called when this application loses the foreground focus.
     *
     * Only the foreground application receives tracking data from the Leap
     * Motion Controller. This function is only called when the controller
     * object is in a connected state.
     *
     *  \include Listener_onFocusLost.txt
     *
     * @param controller The Controller object invoking this callback function.
     */
    LEAP_EXPORT virtual void onFocusLost(const Controller&) {}
};

}

#endif // __Leap_h__
