#include "./VulkanRenderPipeline.hpp"

#include "./VulkanRenderPass.hpp"
#include "./VulkanShader.hpp"
#include "./VulkanBuffer.hpp"

namespace Jade {

	namespace Vulkan {

		static uint32_t s_MaxVertexInputBindingIndex = 0;
		static uint32_t s_NextVertexInputBindingIndex = 0;

		VkFormat GetInputElementFormat(InputElementType type) {
			switch (type) {
				case InputElementType::Vector2: return VK_FORMAT_R32G32_SFLOAT;
				case InputElementType::Vector3: return VK_FORMAT_R32G32B32_SFLOAT;
				case InputElementType::Vector4: return VK_FORMAT_R32G32B32A32_SFLOAT;
				default: return VK_FORMAT_UNDEFINED;
			}
		}
		
		uint32_t GetInputElementSize(InputElementType type) {
			switch (type) {
				case InputElementType::Vector2: return 2 * sizeof(float);
				case InputElementType::Vector3: return 3 * sizeof(float);
				case InputElementType::Vector4: return 4 * sizeof(float);
				default: return 0;
			}
		}

		VulkanRenderPipeline::VulkanRenderPipeline(const Ref<VulkanGraphicsContext> &context, const Ref<VulkanRenderPass> &renderPass, const Ref<VulkanShader> &shader, const std::vector<RenderPipelineInputElement> &elements, const InputLayout &layout) : m_Context(context), m_RenderPass(renderPass), m_Shader(shader) {			
			// TODO: Make pipeline configurable
			
			VkPipelineShaderStageCreateInfo shaderStageInfos[] = {
				{
					.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
					.pNext = nullptr,
					.flags = 0,
					.stage = VK_SHADER_STAGE_VERTEX_BIT,
					.module = shader->GetVertexShaderModule(),
					.pName = "main",
					.pSpecializationInfo = nullptr
				},
				{
					.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
					.pNext = nullptr,
					.flags = 0,
					.stage = VK_SHADER_STAGE_FRAGMENT_BIT,
					.module = shader->GetFragmentShaderModule(),
					.pName = "main",
					.pSpecializationInfo = nullptr
				}
			};
			
			
			VkDynamicState dynamicStates[] = {
				VK_DYNAMIC_STATE_VIEWPORT,
				VK_DYNAMIC_STATE_SCISSOR
			};
			
			VkPipelineDynamicStateCreateInfo dynamicStateInfo = {
				.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
				.pNext = nullptr,
				.flags = 0,
				.dynamicStateCount = JD_ARRAYSIZE(dynamicStates),
				.pDynamicStates = dynamicStates,
			};
			
			uint32_t bindingIndex = s_NextVertexInputBindingIndex;
			if (s_NextVertexInputBindingIndex == s_MaxVertexInputBindingIndex) {
				s_MaxVertexInputBindingIndex++;
			}
			s_NextVertexInputBindingIndex = s_MaxVertexInputBindingIndex;
			
			VkVertexInputBindingDescription bindingDescription = {
				.binding = bindingIndex,
				.stride = (uint32_t)layout.GetTotalSizeBytes(),
				.inputRate = VK_VERTEX_INPUT_RATE_VERTEX
			};
			
			uint32_t attributeCount = layout.GetElementCount();
			VkVertexInputAttributeDescription *attributeDescriptions = Alloc<VkVertexInputAttributeDescription>(attributeCount);
			
			uint32_t offset = 0;
			
			for (uint32_t i = 0; i < attributeCount; i++) {
				InputElement element = layout.GetElement(i);
				attributeDescriptions[i] = {
					.location = i,
					.binding = bindingIndex,
					.format = GetInputElementFormat(element.Type),
					.offset = offset,
				};
				
				offset += GetInputElementSize(element.Type);
			}
			
			VkPipelineVertexInputStateCreateInfo vertexInputInfo = {
				.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
				.pNext = nullptr,
				.flags = 0,
				.vertexBindingDescriptionCount = 1,
				.pVertexBindingDescriptions = &bindingDescription,
				.vertexAttributeDescriptionCount = attributeCount,
				.pVertexAttributeDescriptions = attributeDescriptions,
			};
			
			VkPipelineInputAssemblyStateCreateInfo inputAssembly = {
				.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
				.pNext = nullptr,
				.flags = 0,
				.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
				.primitiveRestartEnable = VK_FALSE
			};
			
			m_Viewport = {
				.x = 0.0f,
				.y = (float)m_Context->GetWindow()->GetHeight(),
				.width = (float)m_Context->GetWindow()->GetWidth(),
				.height = -(float)m_Context->GetWindow()->GetHeight(), // Height is negative to flip the y-axis
				.minDepth = 0.0f,
				.maxDepth = 1.0f,
			};
			
			m_Scissor = {
				.offset = {
					.x = 0,
					.y = 0
				},
				.extent = {
					.width = m_Context->GetWindow()->GetWidth(),
					.height = m_Context->GetWindow()->GetHeight()
				}
			};
			
			VkPipelineViewportStateCreateInfo viewportStateInfo = {
				.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
				.pNext = nullptr,
				.flags = 0,
				.viewportCount = 1,
				.pViewports = &m_Viewport,
				.scissorCount = 1,
				.pScissors = &m_Scissor
			};
			
			VkPipelineRasterizationStateCreateInfo rasterizerInfo = {
				.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
				.pNext = nullptr,
				.flags = 0,
				.depthClampEnable = VK_FALSE,
				.rasterizerDiscardEnable = VK_FALSE,
				.polygonMode = VK_POLYGON_MODE_FILL,
				.cullMode = VK_CULL_MODE_BACK_BIT,
				.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE,
				.depthBiasEnable = VK_FALSE,
				.depthBiasConstantFactor = 0.0f,
				.depthBiasClamp = 0.0f,
				.depthBiasSlopeFactor = 0.0f,
				.lineWidth = 1.0f,
			};
			
			VkPipelineMultisampleStateCreateInfo multisampleInfo = {
				.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
				.pNext = nullptr,
				.flags = 0,
				.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
				.sampleShadingEnable = VK_FALSE,
				.minSampleShading = 1.0f,
				.pSampleMask = nullptr,
				.alphaToCoverageEnable = VK_FALSE,
				.alphaToOneEnable = VK_FALSE,
			};
			
			VkPipelineColorBlendAttachmentState colorBlendAttachment = {
				.blendEnable = VK_TRUE, 
				.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA, 
				.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
				.colorBlendOp = VK_BLEND_OP_ADD,
				.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
				.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
				.alphaBlendOp = VK_BLEND_OP_ADD,
				.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
			};
			
			VkPipelineColorBlendStateCreateInfo colorBlendingInfo = {
				.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
				.pNext = nullptr,
				.flags = 0,
				.logicOpEnable = VK_FALSE,
				.logicOp = VK_LOGIC_OP_COPY,
				.attachmentCount = 1,
				.pAttachments = &colorBlendAttachment,
				.blendConstants = {
					0.0f,
					0.0f, 
					0.0f,
					0.0f
				}
			};

			uint32_t uniformBufferIndex = UINT32_MAX;
			uint32_t samplerIndex = UINT32_MAX;
			std::vector<VkDescriptorPoolSize> poolSizes;

			std::vector<VkDescriptorSetLayoutBinding> bindings(elements.size());
			for (uint32_t i = 0; i < elements.size(); i++) {
				bindings[i].binding = elements[i].BindingIndex;
				bindings[i].descriptorCount = 1;
				bindings[i].stageFlags = VK_SHADER_STAGE_ALL_GRAPHICS;
				bindings[i].pImmutableSamplers = nullptr;

				switch (elements[i].ElementType) {
					case RenderPipelineInputElementType::UniformBuffer: {
						if (uniformBufferIndex == UINT32_MAX) {
							VkDescriptorPoolSize uniformBufferPoolSize;
							uniformBufferPoolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
							uniformBufferPoolSize.descriptorCount = 0;
							
							uniformBufferIndex = poolSizes.size();
							poolSizes.push_back(uniformBufferPoolSize);
						}
						bindings[i].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER; 
						poolSizes[uniformBufferIndex].descriptorCount++;
						break;
					}
					case RenderPipelineInputElementType::Sampler: {
						if (samplerIndex == UINT32_MAX) {
							VkDescriptorPoolSize samplerPoolSize;
							samplerPoolSize.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
							samplerPoolSize.descriptorCount = 0;
							
							samplerIndex = poolSizes.size();
							poolSizes.push_back(samplerPoolSize);
						}
						bindings[i].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER; 
						poolSizes[samplerIndex].descriptorCount++;
						break;
					}
					default: break;
				}
			}

			VkDescriptorSetLayoutCreateInfo descriptorSetLayoutInfo{};
			descriptorSetLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
			descriptorSetLayoutInfo.bindingCount = bindings.size();
			descriptorSetLayoutInfo.pBindings = bindings.data();
			
			JD_VULKAN_CALL(vkCreateDescriptorSetLayout(m_Context->GetLogicalDevice(), &descriptorSetLayoutInfo, nullptr, &m_DescriptorSetLayout));
			
			VkDescriptorPoolCreateInfo poolInfo{};
			poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
			poolInfo.poolSizeCount = poolSizes.size();
			poolInfo.pPoolSizes = poolSizes.data();
			poolInfo.maxSets = 1;

			JD_VULKAN_CALL(vkCreateDescriptorPool(m_Context->GetLogicalDevice(), &poolInfo, nullptr, &m_DescriptorPool));

			VkDescriptorSetAllocateInfo descriptorSetAllocateInfo{};
			descriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
			descriptorSetAllocateInfo.descriptorSetCount = 1;
			descriptorSetAllocateInfo.descriptorPool = m_DescriptorPool;
			descriptorSetAllocateInfo.pSetLayouts = &m_DescriptorSetLayout;
			
			JD_VULKAN_CALL(vkAllocateDescriptorSets(m_Context->GetLogicalDevice(), &descriptorSetAllocateInfo, &m_DescriptorSet));

			VkPipelineLayoutCreateInfo pipelineLayoutInfo = {
				.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
				.pNext = nullptr,
				.flags = 0,
				.setLayoutCount = 1,
				.pSetLayouts = &m_DescriptorSetLayout,
				.pushConstantRangeCount = 0,
				.pPushConstantRanges = nullptr
			};
			
			JD_VULKAN_CALL(vkCreatePipelineLayout(m_Context->GetLogicalDevice(), &pipelineLayoutInfo, nullptr, &m_PipelineLayout));
			
			VkGraphicsPipelineCreateInfo graphicsPipelineInfo = {
				.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
				.pNext = nullptr,
				.flags = 0,
				.stageCount = JD_ARRAYSIZE(shaderStageInfos),
				.pStages = shaderStageInfos,
				.pVertexInputState = &vertexInputInfo,
				.pInputAssemblyState = &inputAssembly,
				.pTessellationState = nullptr,
				.pViewportState = &viewportStateInfo,
				.pRasterizationState = &rasterizerInfo,
				.pMultisampleState = &multisampleInfo,
				.pDepthStencilState = nullptr,
				.pColorBlendState = &colorBlendingInfo,
				.pDynamicState = &dynamicStateInfo,
				.layout = m_PipelineLayout,
				.renderPass = renderPass->GetRenderPass(),
				.subpass = 0,
				.basePipelineHandle = VK_NULL_HANDLE,
				.basePipelineIndex = -1,
			};
			
			JD_VULKAN_CALL(vkCreateGraphicsPipelines(m_Context->GetLogicalDevice(), VK_NULL_HANDLE, 1, &graphicsPipelineInfo, nullptr, &m_Pipeline));
			
			Free(attributeDescriptions);
		}
		
		VulkanRenderPipeline::~VulkanRenderPipeline() {
			vkDestroyDescriptorSetLayout(m_Context->GetLogicalDevice(), m_DescriptorSetLayout, nullptr);
			vkDestroyDescriptorPool(m_Context->GetLogicalDevice(), m_DescriptorPool, nullptr);

			vkDestroyPipeline(m_Context->GetLogicalDevice(), m_Pipeline, nullptr);
			vkDestroyPipelineLayout(m_Context->GetLogicalDevice(), m_PipelineLayout, nullptr);
		}

		void VulkanRenderPipeline::SetInputElement(uint32_t index, const Ref<Native::NativeUniformBuffer> &buffer) {
			Ref<VulkanUniformBuffer> vulkanBuffer = CastRef<VulkanUniformBuffer>(buffer);

			VkDescriptorBufferInfo bufferInfo{};
			bufferInfo.buffer = vulkanBuffer->GetBuffer();
			bufferInfo.offset = 0;
			bufferInfo.range = vulkanBuffer->GetSize();

			VkWriteDescriptorSet descriptorWrite{};
			descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrite.dstSet = m_DescriptorSet;
			descriptorWrite.descriptorCount = 1;
			descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			descriptorWrite.dstArrayElement = 0;
			descriptorWrite.dstBinding = index;
			descriptorWrite.pBufferInfo = &bufferInfo;

			vkUpdateDescriptorSets(m_Context->GetLogicalDevice(), 1, &descriptorWrite, 0, nullptr);
		}
	
	}

	namespace Native {
	
		Ref<NativeRenderPipeline> NativeRenderPipeline::Create(const Ref<NativeGraphicsContext> &context, const Ref<NativeRenderPass> &renderPass, const Ref<NativeShader> &shader, const std::vector<RenderPipelineInputElement> &elements, const InputLayout &layout) {
			return MakeRef<Vulkan::VulkanRenderPipeline>(
				CastRef<Vulkan::VulkanGraphicsContext>(context),
				CastRef<Vulkan::VulkanRenderPass>(renderPass),
				CastRef<Vulkan::VulkanShader>(shader),
				elements,
				layout
			);
		}
	
	}

}