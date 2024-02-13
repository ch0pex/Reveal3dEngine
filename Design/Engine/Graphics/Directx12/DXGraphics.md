Spanish:
1. `viewport_`: Representa la configuración del viewport para renderizar gráficos.
2. `scissorRect_`: Representa el rectángulo de tijera utilizado para recortar durante el renderizado.
3. `swapChain_`: Un puntero al swap chain de DXGI utilizado para presentar los fotogramas renderizados en la pantalla.
4. `device_`: Un puntero al dispositivo Direct3D 12 utilizado para renderizar gráficos.
5. `renderTargets_`: Un array de punteros a los recursos de render target utilizados para el renderizado.
6. `commandAllocator_`: Un puntero al command allocator utilizado para asignar listas de comandos.
7. `commandQueue_`: Un puntero a la cola de comandos utilizada para enviar listas de comandos para su ejecución.
8. `rootSignature_`: Un puntero a la firma raíz utilizada para definir el diseño de los parámetros raíz.
9. `rtvHeap_`: Un puntero al heap de descriptores utilizado para almacenar las vistas de render target.
10. `pipelineState_`: Un puntero al objeto de estado de pipeline utilizado para definir el estado del pipeline gráfico.
11. `commandList_`: Un puntero a la lista de comandos gráficos utilizada para grabar comandos de renderizado.
12. `rtvDescriptorSize_`: El tamaño del descriptor de vista de render target.

Recursos de la aplicación:
13. `vertexBuffer_`: Un puntero al recurso del buffer de vértices utilizado para almacenar datos de vértices.
14. `vertexBufferView_`: Representa la vista del buffer de vértices utilizado para el renderizado.

Objetos de sincronización:
15. `frameIndex_`: El índice del fotograma actual que se está renderizando.
16. `fenceEvent_`: Un identificador del evento utilizado para la sincronización.
17. `fence_`: Un puntero al objeto de fence utilizado para la sincronización.
18. `fenceValue_`: El valor actual del fence utilizado para la sincronización.

English:
1. `viewport_`: Represents the viewport settings for rendering graphics.
2. `scissorRect_`: Represents the scissor rectangle used for clipping during rendering.
3. `swapChain_`: A pointer to the DXGI swap chain used for presenting rendered frames to the screen.
4. `device_`: A pointer to the Direct3D 12 device used for rendering graphics.
5. `renderTargets_`: An array of pointers to the render target resources used for rendering.
6. `commandAllocator_`: A pointer to the command allocator used for allocating command lists.
7. `commandQueue_`: A pointer to the command queue used for submitting command lists for execution.
8. `rootSignature_`: A pointer to the root signature used for defining the layout of root parameters.
9. `rtvHeap_`: A pointer to the descriptor heap used for storing render target views.
10. `pipelineState_`: A pointer to the pipeline state object used for defining the graphics pipeline state.
11. `commandList_`: A pointer to the graphics command list used for recording rendering commands.
12. `rtvDescriptorSize_`: The size of the render target view descriptor.

App resources:
13. `vertexBuffer_`: A pointer to the vertex buffer resource used for storing vertex data.
14. `vertexBufferView_`: Represents the view of the vertex buffer used for rendering.

Synchronization objects:
15. `frameIndex_`: The index of the current frame being rendered.
16. `fenceEvent_`: A handle to the event used for synchronization.
17. `fence_`: A pointer to the fence object used for synchronization.
18. `fenceValue_`: The current value of the fence used for synchronization.