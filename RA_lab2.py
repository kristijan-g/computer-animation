from random import *
from pyrr import Vector3
import numpy as np
from pyglet.gl import *


class Particle:
    def __init__(self, pos):
        self.pos = pos.copy()
        self.vel = Vector3([randrange(-5, 5), randrange(-5, 5), randrange(-5, 5)])
        self.lifeTime = 400

    def update(self, dt):
        self.pos += self.vel * dt * 1000
        self.lifeTime -= 1


class ParticleSystem:
    def __init__(self, texture, width, height, num, size):
        self.texture = texture
        self.particles = []
        self.size = size
        self.generateParticles(num, width, height)
        self.timer = 0

    def generateParticles(self, num, width, height):
        for i in range(0, num):
            p = Particle(Vector3([randrange(-width, width), randrange(-height, height), randrange(-3, 3)]))
            self.particles.append(p)

    def update(self, dt, width, height):
        self.timer += 1
        for p in self.particles:
            p.update(dt)
            if p.lifeTime <= 0 or p.vel == [0, 0, 0]:
                self.particles.remove(p)
        if self.timer == 10:
            self.generateParticles(randint(70, 100), width, height)
            self.timer = 0

    def draw(self):
        glEnable(self.texture.target)
        glBindTexture(self.texture.target, self.texture.id)
        glEnable(GL_BLEND)
        glBlendFunc(GL_ONE, GL_ONE)
        glPushMatrix()
        for p in self.particles:
            matrix = (GLfloat * 16)()
            glGetFloatv(GL_MODELVIEW_MATRIX, matrix)
            matrix = list(matrix)
            CameraUp = np.array([matrix[1], matrix[5], matrix[9]])
            CameraRight = np.array([matrix[0], matrix[4], matrix[8]])
            size = self.size
            v1 = p.pos + CameraRight * size + CameraUp * -size
            v2 = p.pos + CameraRight * size + CameraUp * size
            v3 = p.pos + CameraRight * -size + CameraUp * -size
            v4 = p.pos + CameraRight * -size + CameraUp * size
            glBegin(GL_QUADS)
            glTexCoord2f(0, 0)
            glVertex3f(v3[0], v3[1], v3[2])
            glTexCoord2f(1, 0)
            glVertex3f(v4[0], v4[1], v4[2])
            glTexCoord2f(1, 1)
            glVertex3f(v2[0], v2[1], v2[2])
            glTexCoord2f(0, 1)
            glVertex3f(v1[0], v1[1], v1[2])
            glEnd()
        glDisable(GL_BLEND)
        glPopMatrix()
        glDisable(self.texture.target)


class Window(pyglet.window.Window):
    global pyglet

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.set_minimum_size(600, 400)
        self.source = [0, 0, 0]
        pyglet.clock.schedule_interval(self.update, 1.0 / 1000.0)
        texture = pyglet.image.load('snowflake1.bmp').get_texture()
        self.ParticleSys = ParticleSystem(texture, self.width, self.height, 50, 70)

    def update(self, dt):
        self.ParticleSys.update(dt, self.width, self.height)

    def on_draw(self):
        self.clear()
        glClear(GL_COLOR_BUFFER_BIT)
        lookAt = [500, 0, 0]
        camera_position = [1500, 1500, 1500]
        glMatrixMode(GL_PROJECTION)
        glLoadIdentity()
        gluPerspective(70, self.width / self.height, 0.05, 10000)
        glMatrixMode(GL_MODELVIEW)
        glLoadIdentity()
        gluLookAt(camera_position[0], camera_position[1], camera_position[2], lookAt[0], lookAt[1], lookAt[2], 0.0, 1.0, 0.0)
        glPushMatrix()
        self.ParticleSys.draw()
        glPopMatrix()
        glFlush()


window = Window(width=1200, height=650, caption='Generator čestica - snježna oluja', resizable=True)
pyglet.gl.glClearColor(0, 0, 0, 1)
pyglet.app.run()
