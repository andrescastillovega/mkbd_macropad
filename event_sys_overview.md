# ZMK Event System Overview

## ZMK Event System Overview

ZMK uses a sophisticated event-driven architecture that enables decoupled communication between keyboard subsystems. Events are raised when system state changes and dispatched to all registered listeners.

**Core Pattern**:
- Events are declared with `ZMK_EVENT_DECLARE()` and implemented with `ZMK_EVENT_IMPL()`
- Listeners subscribe using `ZMK_SUBSCRIPTION(module, event_type)`
- Special `ZMK_DISPLAY_WIDGET_LISTENER` macro provides thread-safe display updates

## Complete List of Available ZMK Events

### Battery & Power Events
- **`zmk_battery_state_changed`** - Battery percentage changes (0-100%)
- **`zmk_peripheral_battery_state_changed`** - Split keyboard peripheral battery status
- **`zmk_usb_conn_state_changed`** - USB connection state (NONE/POWERED/HID)
- **`zmk_activity_state_changed`** - Activity state (ACTIVE/IDLE/SLEEP)

### Input Events  
- **`zmk_position_state_changed`** - Raw key matrix position press/release
- **`zmk_keycode_state_changed`** - Processed keycode events with modifiers
- **`zmk_sensor_event`** - Encoder/rotary sensor data
- **`zmk_mouse_button_state_changed`** - Mouse button press/release

### State Management Events
- **`zmk_layer_state_changed`** - Layer activation/deactivation
- **`zmk_modifiers_state_changed`** - Modifier key state (Ctrl, Alt, Shift)
- **`zmk_hid_indicators_changed`** - HID indicators (Caps Lock, Num Lock)

### Connectivity Events
- **`zmk_ble_active_profile_changed`** - Bluetooth profile switches (0-4)
- **`zmk_endpoint_changed`** - Output endpoint changes (USB ↔ BLE)
- **`zmk_split_peripheral_status_changed`** - Split keyboard connection status

### Metrics Events
- **`zmk_wpm_state_changed`** - Typing speed (words per minute)

## Event Processing Flow

1. **Raise**: `raise_zmk_event_type(event_data)`
2. **Dispatch**: Event manager iterates through subscribers
3. **Handle**: Listeners return:
   - `ZMK_EV_EVENT_BUBBLE` (0) - Continue to next listener
   - `ZMK_EV_EVENT_HANDLED` (1) - Stop propagation  
   - `ZMK_EV_EVENT_CAPTURED` (2) - Stop propagation

## Event System Architecture

### Core Event Infrastructure

**Event Manager**: Central event dispatching system with thread-safe event handling and proper ordering.

**Event Definition Pattern**:
Every event follows a consistent pattern using macros:
- `ZMK_EVENT_DECLARE(event_type)` - Declares an event type in header files
- `ZMK_EVENT_IMPL(event_type)` - Implements the event type in source files
- `ZMK_LISTENER(mod, cb)` - Defines an event listener
- `ZMK_SUBSCRIPTION(mod, ev_type)` - Subscribes a listener to an event type

### Display Widget Event Integration

ZMK provides a special macro `ZMK_DISPLAY_WIDGET_LISTENER` for creating thread-safe display widgets that respond to events:

```c
ZMK_DISPLAY_WIDGET_LISTENER(listener_name, state_type, update_callback, state_getter)
```

This macro:
- Creates thread-safe state management with mutexes
- Handles event dispatching to the display work queue
- Provides proper LVGL context for UI updates
- Generates initialization functions automatically

### Example Usage Pattern:
```c
// Subscribe to events
ZMK_SUBSCRIPTION(widget_battery_status, zmk_battery_state_changed);
ZMK_SUBSCRIPTION(widget_battery_status, zmk_usb_conn_state_changed);

// Define widget listener
ZMK_DISPLAY_WIDGET_LISTENER(widget_battery_status, struct battery_status_state,
                            battery_status_update_cb, battery_status_get_state)
```

## Event Subscription Patterns

### Single Event Subscription:
```c
ZMK_SUBSCRIPTION(module_name, event_type);
```

### Multiple Event Subscriptions:
```c
ZMK_SUBSCRIPTION(widget_battery_status, zmk_battery_state_changed);
ZMK_SUBSCRIPTION(widget_battery_status, zmk_usb_conn_state_changed);
```

### Custom Event Handling:
```c
static int my_event_listener(const zmk_event_t *eh) {
    struct zmk_battery_state_changed *battery_ev = as_zmk_battery_state_changed(eh);
    if (battery_ev != NULL) {
        // Handle battery event
        return ZMK_EV_EVENT_BUBBLE;
    }
    return ZMK_EV_EVENT_BUBBLE;
}

ZMK_LISTENER(my_module, my_event_listener);
ZMK_SUBSCRIPTION(my_module, zmk_battery_state_changed);
```

## Detailed Event Data Structures

### Battery State Event
```c
struct zmk_battery_state_changed {
    uint8_t state_of_charge; // Battery percentage (0-100)
};
```

### USB Connection State Event
```c
struct zmk_usb_conn_state_changed {
    enum zmk_usb_conn_state conn_state; // NONE, POWERED, HID
};
```

### Layer State Event
```c
struct zmk_layer_state_changed {
    uint8_t layer;      // Layer index
    bool state;         // Active (true) or inactive (false)
    int64_t timestamp;  // Event timestamp
};
```

### Position State Event
```c
struct zmk_position_state_changed {
    uint8_t source;      // Event source (local or remote)
    uint32_t position;   // Key position in matrix
    bool state;          // Pressed or released
    int64_t timestamp;   // Event timestamp
};
```

### Keycode State Event
```c
struct zmk_keycode_state_changed {
    uint16_t usage_page;           // HID usage page
    uint32_t keycode;              // HID keycode
    uint8_t implicit_modifiers;    // Auto-applied modifiers
    uint8_t explicit_modifiers;    // User-specified modifiers
    bool state;                    // Pressed (true) or released (false)
    int64_t timestamp;             // Event timestamp
};
```

## Key Takeaways

1. **Event-Driven Architecture**: ZMK uses events to decouple components and enable reactive behavior
2. **Thread Safety**: Display widgets use special macros for thread-safe event handling
3. **Comprehensive Coverage**: Events cover all major keyboard subsystems
4. **Consistent Patterns**: All events follow the same declaration/implementation pattern
5. **Flexible Subscription**: Components can subscribe to multiple events as needed
6. **Hierarchical Processing**: Events support bubbling, handling, and capturing semantics

This event system enables ZMK's modular architecture and makes it possible to create responsive displays, power management, and complex keyboard behaviors.