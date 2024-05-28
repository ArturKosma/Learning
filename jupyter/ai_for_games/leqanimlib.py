import matplotlib.animation
import matplotlib.pyplot as plt
from itertools import count
import random
import time
import numpy as np

plt.rcParams["animation.html"] = "jshtml"
plt.rcParams['figure.dpi'] = 150  
plt

class leqanimlib:

    class dot:
        def __init__(self, x=0, y=0, angle=0):
            self.pos = np.array([x, y], dtype=float)
            self.set_angle(angle)

        def set_angle(self, angle):
            self.angle = angle

        def set_pos(self, pos):
            self.pos = pos

        def get_angle(U, V):
            angle_rad = np.arctan2(V, U)
            angle_deg = np.degrees(angle_rad)
            return angle_deg

        def get_orientation(self):
            angle_rad = np.radians(self.angle)
            return np.array([np.cos(angle_rad), np.sin(angle_rad)], dtype=float)

    def __init__(self, xlim_min=-1, xlim_max=1, ylim_min=-1, ylim_max=1, frame_interval=16, frame_num=60, arrow_length=0.1):
        
        # Anim params.
        self.total_frames = frame_num
        self.frame_interval = frame_interval

        # Objects.
        self.objects = []

        # Time params.
        self.start_time = time.time()
        self.prev_time = time.time()
        self.delta_time = 0

        # Matplotlib setup.
        self.fig, self.ax = plt.subplots()
        self.ax.set_xlim(xlim_min, xlim_max)
        self.ax.set_ylim(ylim_min, ylim_max)
        self.scatter = self.ax.scatter([], [], c='r')
        self.arrow_length = arrow_length

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

    def add_dot(self, x=0, y=0, angle=0):
        new_dot = leqanimlib.dot(x, y, angle)
        self.objects.append(new_dot)
        return new_dot

    def initialize_objects(self):
        
        if len(self.objects) > 0:
            positions = np.array([dot.pos for dot in self.objects])
            orientations = np.array([dot.get_orientation() for dot in self.objects])
            self.quiver = self.ax.quiver(positions[:, 0], positions[:, 1], orientations[:, 0], orientations[:, 1], scale=5)
            
            # Initialize and store texts.
            self.object_texts = []
            for pos in positions:
                text = self.ax.text(pos[0], pos[1], 'test', fontsize=6, verticalalignment='top', horizontalalignment='center')
                self.object_texts.append(text)

    def orientations_to_quiver(self):
        if len(self.objects) > 0:
            positions = np.array([dot.pos for dot in self.objects])
            orientations = np.array([dot.get_orientation() for dot in self.objects])
            u, v = orientations[:, 0], orientations[:, 1]
            # Normalize orientations.
            lengths = np.sqrt(u**2 + v**2)
            u /= lengths
            v /= lengths
            u *= self.arrow_length
            v *= self.arrow_length
            return positions[:, 0], positions[:, 1], u, v
        else:
            return np.array([]), np.array([]), np.array([]), np.array([])

    def internal_tick(self, t, update_func):

        # Debug.
        curr_time = time.time()
        self.delta_time = curr_time - self.prev_time
        if self.delta_time == 0:
            self.delta_time = self.frame_interval / 1000
        if t == 0:
            self.delta_time = 0
        elapsed_time = curr_time - self.start_time
        
        fps = 1 / self.delta_time if self.delta_time > 0 else float('inf')

        # Update text annotations.
        self.texts["fps"].set_text(f"FPS: {fps:.0f}")
        self.texts["elapsed_time"].set_text(f"Time Elapsed: {elapsed_time:.2f}s")
        self.texts["delta_time"].set_text(f"Delta Time: {self.delta_time:.3f}s")
        self.texts["frame"].set_text(f"Frame: {t}")

        # Update objects.
        if len(self.objects) > 0:
            
            # Set positions and orientations.
            x, y, u, v = self.orientations_to_quiver()
            self.scatter.set_offsets(np.c_[x, y])
            self.quiver.set_offsets(np.c_[x, y])
            self.quiver.set_UVC(u, v)

            # Set objects texts locations.
            for i, text in enumerate(self.object_texts):
                text.set_position((x[i], y[i]))
                text.set_text(f'[{x[i]:.2f}, {y[i]:.2f}], {leqanimlib.dot.get_angle(u[i], v[i]):.0f}')

            # User tick.
            update_func(t)

        self.prev_time = curr_time

        return

    def draw(self, update_func):
        
        # Initialize all the animation objects.
        self.initialize_objects()

        # Size.
        self.fig.set_size_inches(8, 6)

        # Update function.
        anim = matplotlib.animation.FuncAnimation(self.fig, lambda frame: self.internal_tick(frame, update_func), 
                                                  frames=range(self.total_frames), interval=self.frame_interval)
        plt.close()
        plt.show()

        return anim