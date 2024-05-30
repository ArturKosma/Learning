import matplotlib.animation
import matplotlib.pyplot as plt
from itertools import count
import random
import time
import numpy as np
import leqmath
from enum import Enum

plt.rcParams["animation.html"] = "jshtml"
plt.rcParams['figure.dpi'] = 150  
plt

class leqanimlib:

    class BaseObject:

        class MovementMode(Enum):
            ROT_FOLLOW_VELOCITY = "rot_follow_velocity"
            ROT_FOLLOW_FOCUS = "rot_follow_focus"
            ROT_MANUAL = "rot_manual"
        
        def __init__(self, x=0, y=0, angle=0, display_name=''):
            if isinstance(x, np.ndarray) and x.shape == (2,):
                self.pos = x.astype(float)
            else:
                self.pos = np.array([x, y], dtype=float)
            self.set_angle(angle)
            self.vel = np.array([0, 0], dtype=float)
            self.rotate_mode = leqanimlib.BaseObject.MovementMode.ROT_FOLLOW_VELOCITY
            
            self.scatter = None
            self.quiver = None
            self.text = None
            self.color = 'black'
            self.display_name = display_name
            self.transform_text = True
            self.transform_text_pos = False
            self.transform_text_angle = False
            self.transform_text_velocity = False
            self.max_speed = float('inf')
            self.ghost_enabled = False

        def update(self, delta_time):
            current_speed = np.linalg.norm(self.vel)
            if current_speed > self.max_speed:
                self.vel /= current_speed
                self.vel *= self.max_speed
            self.set_pos(self.pos + (self.vel * delta_time))

            if self.rotate_mode == leqanimlib.BaseObject.MovementMode.ROT_FOLLOW_VELOCITY:
                self.set_angle(leqmath.leqmath.dir_to_angle(self.vel[0], self.vel[1]))
                
            elif self.rotate_mode == leqanimlib.BaseObject.MovementMode.ROT_FOLLOW_FOCUS:
                pass

            elif self.rotate_mode == leqanimlib.BaseObject.MovementMode.ROT_MANUAL:
                pass

        def display(self, ax, x, y, u, v):

            text_pos = f'position: [{x:.2f}, {y:.2f}]'
            text_angle = f'angle: {leqmath.leqmath.dir_to_angle(u, v):.0f}'
            text_display_name = f'{self.display_name}'
            text_velocity = f'velocity: [{self.vel[0]:.2f}, {self.vel[1]:.2f}]'

            # Create text if non-existent.
            if self.text is None:
                self.text = ax.text(x, y, f'', color=self.color)
            else:
                self.text.set_text(f'')
            
            # Position of the text.
            self.text.set_position((x, y))

            # Add all other texts if enabled.

            if self.transform_text_pos == True:
                current_text = self.text.get_text()
                new_text = f'{text_pos}\n{current_text}'
                self.text.set_text(new_text)

            if self.transform_text_angle == True:
                current_text = self.text.get_text()
                new_text = f'{text_angle}\n{current_text}'
                self.text.set_text(new_text)

            if self.transform_text_velocity == True:
                current_text = self.text.get_text()
                new_text = f'{text_velocity}\n{current_text}'
                self.text.set_text(new_text)

            # If transform texts disabled, hide them.
            if self.transform_text == False:
                self.text.set_text(f'')

            # Add display name if it exists.
            if self.display_name:
                current_text = self.text.get_text()
                new_text = f'{text_display_name}\n{current_text}'
                self.text.set_text(new_text)

            # Ghost trail.
            if self.ghost_enabled == True:
                ax.scatter(x, y, color=self.color, alpha=0.2)

        def set_angle(self, angle):
            self.angle = angle

        def set_pos(self, pos):
            self.pos = pos

        def get_orientation(self):
            angle_rad = np.radians(self.angle)
            return np.array([np.cos(angle_rad), np.sin(angle_rad)], dtype=float)

        def set_vel(self, vel):
            self.vel = vel;

        def set_color(self, color):
            self.color = color

    class Dot(BaseObject):
        def __init__(self, x=0, y=0, angle=0, display_name=''):
            super().__init__(x, y, angle, display_name)
            self.arrow_enabled = True
            
        def display(self, ax, x, y, u, v):
            super().display(ax, x, y, u, v)
            if self.scatter is None:
                self.scatter = ax.scatter(x, y, color=self.color)
            else:
                self.scatter.set_offsets(np.c_[x, y])

            if self.arrow_enabled == True:
                if self.quiver is None:
                    self.quiver = ax.quiver(x, y, u, v, color=self.color)
                else:
                    self.quiver.set_offsets(np.c_[x, y])
                    self.quiver.set_UVC(u, v)
            else:
                self.quiver = None
                
        def set_arrow_enabled(self, enabled):
            self.arrow_enabled = enabled

    class Arrow(BaseObject):
        def __init__(self, x=0, y=0, angle=0, length=1, display_name=''):
            super().__init__(x, y, angle, display_name)
            self.length = length
            self.transform_text = False

        def set_length(self, length):
            self.length = length

        def display(self, ax, x, y, u, v):
            super().display(ax, x, y, u, v)
            
            if self.quiver is None:
                self.quiver = ax.quiver(x, y, u, v, color=self.color)
            else:
                self.quiver.set_offsets(np.c_[x, y])
                self.quiver.set_UVC(u, v)

    def __init__(self, xlim_min=-1, xlim_max=1, ylim_min=-1, ylim_max=1, frame_interval=16, frame_num=60, title=''):
        
        # Anim params.
        self.total_frames = frame_num

        # Objects.
        self.objects = []

        # Time params.
        self.delta_time = frame_interval / 1000
        self.time_elapsed = 0

        # Matplotlib setup.
        self.fig, self.ax = plt.subplots()
        self.ax.set_xlim(xlim_min, xlim_max)
        self.ax.set_ylim(ylim_min, ylim_max)
        self.ax.set_title(title)

        # Text Annotations
        self.texts = {
            "fps": self.ax.text(0.95, 0.9, "", transform=self.ax.transAxes, fontsize=8,
                                verticalalignment='top', horizontalalignment='right'),
            "elapsed_time": self.ax.text(0.95, 0.87, "", transform=self.ax.transAxes, fontsize=8,
                                         verticalalignment='top', horizontalalignment='right'),
            "delta_time": self.ax.text(0.95, 0.84, "", transform=self.ax.transAxes, fontsize=8,
                                       verticalalignment='top', horizontalalignment='right'),
            "frame": self.ax.text(0.95, 0.81, "", transform=self.ax.transAxes, fontsize=8,
                                  verticalalignment='top', horizontalalignment='right')
        }

    def set_title(self, title):
        self.ax.set_title(title)

    def add_object(self, obj):
        self.objects.append(obj)
        return obj

    def get_objects_transforms(self):
        if len(self.objects) > 0:
            positions = np.array([obj.pos for obj in self.objects])
            orientations = np.array([obj.get_orientation() for obj in self.objects])
            u, v = orientations[:, 0], orientations[:, 1]
            # Normalize orientations.
            lengths = np.sqrt(u**2 + v**2)
            u /= lengths
            v /= lengths
            return positions[:, 0], positions[:, 1], u, v
        else:
            return np.array([]), np.array([]), np.array([]), np.array([])

    def reset_time(self):
        self.time_elapsed = 0

    def internal_tick(self, t, update_func):
        
        # Count time.
        self.time_elapsed += self.delta_time

        # Update text annotations.
        self.texts["elapsed_time"].set_text(f"Time Elapsed: {self.time_elapsed:.2f}s")
        self.texts["delta_time"].set_text(f"Delta Time: {self.delta_time:.3f}s")
        self.texts["frame"].set_text(f"Frame: {t}")

        # Update.
        if len(self.objects) > 0:

            # User tick.
            update_func(t)
            
            # Update each object.
            for object in self.objects:
                object.update(self.delta_time)
            
            # Display each object.
            x, y, u, v = self.get_objects_transforms()
            for i, obj in enumerate(self.objects):
                obj.display(self.ax, x[i], y[i], u[i], v[i])

        return

    def draw(self, update_func):

        # Viewport size.
        self.fig.set_size_inches(8, 6)

        # Update function.
        anim = matplotlib.animation.FuncAnimation(self.fig, lambda frame: self.internal_tick(frame, update_func), 
                                                  frames=range(self.total_frames), interval=self.delta_time * 1000)
        plt.close()
        plt.show()

        return anim