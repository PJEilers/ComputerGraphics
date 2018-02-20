//Change contents of mainview

/*mainviewAlt::mainviewAlt()
{
    vertex cube[8];
            cube[0].x = -1.0;
            cube[0].y = -1.0;
            cube[0].z = -1.0;
            cube[0].nx = 0.0;
            cube[0].ny = 0.0;
            cube[0].nz = 1.0;
            cube[0].s0 = 0.0;
            cube[0].t0 = 0.0;
            cube[1].x = 1.0;
            cube[1].y = -1.0;
            cube[1].z = -1.0;
            cube[1].nx = 0.0;
            cube[1].ny = 0.0;
            cube[1].nz = 1.0;
            cube[1].s0 = 1.0;
            cube[1].t0 = 0.0;
            cube[2].x = 1.0;
            cube[2].y = 1.0;
            cube[2].z = -1.0;
            cube[2].nx = 0.0;
            cube[2].ny = 0.0;
            cube[2].nz = 1.0;
            cube[2].s0 = 0.0;
            cube[2].t0 = 1.0;
            cube[3].x = -1.0;
            cube[3].y = 1.0;
            cube[3].z = -1.0;
            cube[3].nx = 0.0;
            cube[3].ny = 0.0;
            cube[3].nz = 1.0;
            cube[3].s0 = 1.0;
            cube[3].t0 = 1.0;
            cube[4].x = -1.0;
            cube[4].y = -1.0;
            cube[4].z = 1.0;
            cube[4].nx = 0.0;
            cube[4].ny = 0.0;
            cube[4].nz = 0.0;
            cube[4].s0 = 0.0;
            cube[4].t0 = 0.0;
            cube[5].x = 1.0;
            cube[5].y = -1.0;
            cube[5].z = 1.0;
            cube[5].nx = 0.0;
            cube[5].ny = 0.0;
            cube[5].nz = 0.0;
            cube[5].s0 = 1.0;
            cube[5].t0 = 0.0;
            cube[6].x = 1.0;
            cube[6].y = 1.0;
            cube[6].z = 1.0;
            cube[6].nx = 0.0;
            cube[6].ny = 0.0;
            cube[6].nz = 0.0;
            cube[6].s0 = 0.0;
            cube[6].t0 = 1.0;
            cube[7].x = -1.0;
            cube[7].y = 1.0;
            cube[7].z = 1.0;
            cube[7].nx = 0.0;
            cube[7].ny = 0.0;
            cube[7].nz = 0.0;
            cube[7].s0 = 1.0;
            cube[7].t0 = 1.0;

           vertex pyramid[5];
                pyramid[0].x = -1.0;
                pyramid[0].y = -1.0;
                pyramid[0].z = -1.0;
                pyramid[0].nx = 0.0;
                pyramid[0].ny = 0.0;
                pyramid[0].nz = 1.0;
                pyramid[0].s0 = 0.0;
                pyramid[0].t0 = 0.0;
                pyramid[1].x = 1.0;
                pyramid[1].y = -1.0;
                pyramid[1].z = -1.0;
                pyramid[1].nx = 0.0;
                pyramid[1].ny = 0.0;
                pyramid[1].nz = 1.0;
                pyramid[1].s0 = 0.0;
                pyramid[1].t0 = 0.0;
                pyramid[2].x = -1.0;
                pyramid[2].y = -1.0;
                pyramid[2].z = 1.0;
                pyramid[2].nx = 0.0;
                pyramid[2].ny = 0.0;
                pyramid[2].nz = 0.0;
                pyramid[2].s0 = 0.0;
                pyramid[2].t0 = 0.0;
                pyramid[3].x = 1.0;
                pyramid[3].y = -1.0;
                pyramid[3].z = -1.0;
                pyramid[3].nx = 0.0;
                pyramid[3].ny = 0.0;
                pyramid[3].nz = 0.0;
                pyramid[3].s0 = 0.0;
                pyramid[3].t0 = 0.0;
                pyramid[4].x = 0.0;
                pyramid[4].y = 1.0;
                pyramid[4].z = 0.0;
                pyramid[4].nx = 0.0;
                pyramid[4].ny = 0.0;
                pyramid[4].nz = 0.0;
                pyramid[4].s0 = 1.0;
                pyramid[4].t0 = 1.0;

                cube4x4m.translate(QVector3D(2,0,-6));
                pyramid4x4m.translate(QVector3D(-2,0,-6));
                projection.perspective(60, 1.4, 1, 100);


                //Initializing cube

                ushort pindicesC[] = { 0, 1, 2,   2, 3, 0,      // front
                                       4, 5, 6,   6, 7, 4,      // right
                                       8, 9,10,  10,11, 8,      // top
                                      12,13,14,  14,15,12,      // left
                                      16,17,18,  18,19,16,      // bottom
                                      20,21,22,  22,23,20 };    // back

                glGenBuffers(12, vboCube);
                glBindBuffer(GL_ARRAY_BUFFER, *vboCube);
                glBufferData(GL_ARRAY_BUFFER, sizeof(vertex)*8, cube, GL_STATIC_DRAW);
                glGenVertexArrays(12, vaoCube);
                glBindVertexArray(*vaoCube);
                glEnableVertexAttribArray(0);
                glEnableVertexAttribArray(1);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), BUFFER_OFFSET(0));
                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), BUFFER_OFFSET(12));

                glGenBuffers(36, iboCube);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *iboCube);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertex)*8, pindicesC, GL_STATIC_DRAW);

                //Initializing pyramid

                ushort pindicesP[] = { 0, 1, 2,   2, 1, 3,
                                       3, 1, 4,   4, 1, 0,
                                       0, 3, 4,  0, 2, 3};

                glGenBuffers(6, vboPyramid);
                glBindBuffer(GL_ARRAY_BUFFER, *vboPyramid);
                glBufferData(GL_ARRAY_BUFFER, sizeof(vertex)*5, pyramid, GL_STATIC_DRAW);
                glGenVertexArrays(6, vaoPyramid);
                glBindVertexArray(*vaoPyramid);
                glEnableVertexAttribArray(0);
                glEnableVertexAttribArray(1);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), BUFFER_OFFSET(0));
                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), BUFFER_OFFSET(12));

                glGenBuffers(18, iboPyramid);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *iboPyramid);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertex)*5, pindicesP, GL_STATIC_DRAW);
    }







    void MainView::paintGL() {
        // Clear the screen before rendering
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        shaderProgram.bind();

        // Draw here
        glUniformMatrix4fv(rotationLoc, 1, GL_FALSE, rotation.data());
        glUniformMatrix4fv(scalingLoc, 1, GL_FALSE, scaling.data());
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, cube4x4m.data());
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, projection.data());

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *iboCube);
      //  glBindVertexArray(*vaoCube);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, BUFFER_OFFSET(0));

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, pyramid4x4m.data());

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *iboPyramid);
    //    glBindVertexArray(*vaoPyramid);
        glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_SHORT, BUFFER_OFFSET(0));

        shaderProgram.release();
    }

}*/
