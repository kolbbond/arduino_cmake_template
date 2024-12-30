-- to override clangd startup in this directory
local nvim_lsp = require('lspconfig')

--[[
nvim_lsp.arduino_language_server.setup {
    root_dir = function()
        return vim.fn.getcwd()
    end,
    filetypes = { 'cpp' }
}
--]]
