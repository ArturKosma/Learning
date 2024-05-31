import matplotlib.animation
import matplotlib.pyplot as plt
from itertools import count
import random
import time
import numpy as np
import leqmath
from enum import Enum
import gc

plt.rcParams["animation.html"] = "jshtml"
plt.rcParams['figure.dpi'] = 150  
plt

class leqanimlib:

    class FlockingGroup:

        def __init__(self, members, k):
            self.members = members
            self.k = k
            self.separation_rate = 1/5
            self.cohesion_rate = 2/5
            self.alignment_rate = 2/5

        def get_other_members(self, caller):
            return [member for member in self.members if member != caller]

    class BaseObject:

        class RotateMode(Enum):
            ROT_FOLLOW_VELOCITY = "rot_follow_velocity"
            ROT_FOLLOW_FOCUS = "rot_follow_focus"
            ROT_MANUAL = "rot_manual"

        class MovementMode(Enum):
            MOV_MANUAL = "mov_manual"
            MOV_FLOCKING = "mov_flocking"
        
        def __init__(self, x=0, y=0, angle=0, display_name='', color='black'):
            if isinstance(x, np.ndarray) and x.shape == (2,):
                self.pos = x.astype(float)
            else:
                self.pos = np.array([x, y], dtype=float)

            # Cache world.
            self.world = None
            if self.world == None:
                self.world = next((obj for obj in gc.get_objects() if isinstance(obj, leqanimlib)), None)
                
            self.set_angle(angle)
            self.vel = np.array([0, 0], dtype=float)
            self.acc = 10
            self.radius = 1.0
            
            self.movement_mode = leqanimlib.BaseObject.MovementMode.MOV_MANUAL
            self.rotate_mode = leqanimlib.BaseObject.RotateMode.ROT_FOLLOW_VELOCITY
            self.max_speed = 5
            self.focus_target = None
            self.flocking_group = None
            self.avoidance = False
            self.goal = np.array([])
            
            self.sight_distance = 5.0
            self.sight_half_angle = 45.0
            
            self.scatter = None
            self.quiver = None
            self.text = None
            self.color = color
            self.display_name = display_name
            self.transform_text = True
            self.transform_text_pos = False
            self.transform_text_angle = False
            self.transform_text_velocity = False
            self.ghost_enabled = False

        def update(self, t, delta_time):
            
            # Based on movement mode.
            if self.movement_mode == leqanimlib.BaseObject.MovementMode.MOV_MANUAL:
                pass
                
            elif self.movement_mode == leqanimlib.BaseObject.MovementMode.MOV_FLOCKING:
                other_members = self.flocking_group.get_other_members(self)

                # Separation.
                k = self.flocking_group.k
                separation = np.array([0, 0], dtype=float)

                # Cohesion.
                cohesion = np.array([0, 0], dtype=float)
                center = np.array([0, 0], dtype=float)

                # Alignment.
                alignment = np.array([0, 0], dtype=float)    

                # Loop all other members.
                for member in other_members:

                    # Separation.
                    direction = self.pos - member.pos
                    direction /= np.linalg.norm(direction)
                    dist = np.linalg.norm(self.pos - member.pos)
                    strength = k / (dist * dist) # Inverse square law.
                    separation += strength * direction

                    # Cohesion.
                    center += member.pos

                    # Alignment.
                    alignment += member.vel

                # Cohesion.
                center /= len(other_members)
                cohesion = center - self.pos

                # Sum flocking.
                separation *= self.flocking_group.separation_rate
                cohesion *= self.flocking_group.cohesion_rate
                alignment *= self.flocking_group.alignment_rate
                self.vel += separation + cohesion + alignment

            # Accelerate if goal is given.
            if self.goal != np.array([]):
                dir_goal = self.goal - self.pos
                dir_goal /= np.linalg.norm(dir_goal)
                self.vel += dir_goal * self.acc * delta_time

            # Avoidance.
            if self.avoidance == True:
                
                # All currently seen objects:
                seen_objects = self.get_sight()

                # Store closest colliding.
                shortest_time = float('inf') # Time of shortest collision. After this time we will collide first.
                first_target = None # Object of first collision.
                first_min_separation = 0.0 # Distance to the first collision at shortest approach.
                first_distance = 0.0 # Current distance to first collision.
                first_relative_pos = np.array([]) # Target.pos - Character.pos.
                first_relative_vel = np.array([]) # Target.vel - Character.vel.

                # Loop through all in sight.
                for object in seen_objects:

                    if object.avoidance == False:
                        continue
                    
                    # Calculate the time to collision.
                    relative_pos = object.pos - self.pos
                    relative_vel = object.vel - self.vel
                    relative_speed = np.linalg.norm(relative_vel)
                    time_to_collision = np.dot(relative_pos, relative_vel) / (relative_speed * relative_speed) * -1

                    # Check if there's going to be a collision at all.
                    dist = np.linalg.norm(relative_pos)
                    min_separation = dist - relative_speed * time_to_collision
                    if min_separation <= 2 * self.radius:

                        # Check if it is the shortest.
                        if time_to_collision > 0 and time_to_collision < shortest_time:
                            shortest_time = time_to_collision
                            first_target = object
                            first_min_separation = min_separation
                            first_distance = dist
                            first_relative_pos = relative_pos
                            first_relative_vel = relative_vel

                if first_target != None:

                    # If we’re going to hit exactly, or if we’re already colliding, do steering based on current position.
                    if first_min_separation <= 0 or first_distance < 2 * self.radius:
                        relative_pos = first_target.pos - self.pos

                    # Otherwise calculate the evasion based on predicted collision location.
                    else:                    
                        relative_pos = first_relative_pos + first_relative_vel * shortest_time

                    # Get evasion direction.
                    evasion_dir = -relative_pos / np.linalg.norm(relative_pos)

                    self.vel += evasion_dir * self.acc * delta_time

            # Switch rotate mode.
            if self.rotate_mode == leqanimlib.BaseObject.RotateMode.ROT_FOLLOW_VELOCITY:
                self.set_angle(leqmath.leqmath.dir_to_angle(self.vel[0], self.vel[1]))
                
            elif self.rotate_mode == leqanimlib.BaseObject.RotateMode.ROT_FOLLOW_FOCUS:
                if self.focus_target != None:
                    direction = self.focus_target - self.pos
                    self.set_angle(direction)

            elif self.rotate_mode == leqanimlib.BaseObject.RotateMode.ROT_MANUAL:
                pass

            # Clamp max speed.
            current_speed = np.linalg.norm(self.vel)
            if current_speed > self.max_speed:
                self.vel /= current_speed
                self.vel *= self.max_speed

            # Set new position.
            self.set_pos(self.pos + (self.vel * delta_time))

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

        def get_sight(self):

            seen_objects = []
            
            if self.world:
                
                # Check if there's any object in frontal cone.
                for obj in self.world.objects:
                    dir = obj.pos - self.pos
                    distsq = (dir[0] * dir[0]) + (dir[1] * dir[1])
                    # Distance.
                    if distsq <= (self.sight_distance * self.sight_distance):
                        dist = np.linalg.norm(dir)
                        if dist > 0:
                            dir /= dist
                            selfdir = leqmath.leqmath.angle_to_dir(self.angle)
                            dot = np.dot(dir, selfdir)
                            dot = np.clip(dot, -1.0, 1.0)
                            angle = np.degrees(np.arccos(dot))
                            # Angle.
                            if angle <= self.sight_half_angle:
                                seen_objects.append(obj)
                            
            return seen_objects

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
        def __init__(self, x=0, y=0, angle=0, display_name='', color='black'):
            super().__init__(x, y, angle, display_name, color)
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
        def __init__(self, x=0, y=0, angle=0, length=1, display_name='', color='black'):
            super().__init__(x, y, angle, display_name, color)
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
                object.update(t, self.delta_time)
            
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